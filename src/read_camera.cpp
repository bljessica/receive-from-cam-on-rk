#include "read_camera.h"
#include "rockface_control.h"

// time_t now;
// struct tm *t;


// 读取rtsp流
void ReadRtsp(SocketClient socket_client) {
    AVFormatContext *fmt_ctx = NULL;
    const AVCodec *codec;
    AVCodecContext *codec_ctx = NULL;
    AVStream *stream = NULL;
    AVPacket av_packet;
    AVFrame *frame;

    AVDictionary *ops = NULL;
    // 强制使用tcp，udp在1080p时会丢包导致花屏
    av_dict_set(&ops, "rtsp_transport", "tcp", 0); // tcp更可靠但速度更慢
    av_dict_set(&ops, "max_delay", "5000000", 0); // 设置最大时延5s
    av_dict_set(&ops, "strict", "1", 0); // 使用更高的重排序计数
    av_dict_set(&ops, "buffer_size", "1024000", 0); // 设置最大缓冲
    // av_dict_set(&ops, "max_delay", "500000", 0); 
    // av_dict_set(&ops, "framerate", "20", 0); // 码率
    // 打开网络流
    if (avformat_open_input(&fmt_ctx, RTSP_URL, NULL, &ops) != 0) {
    // if (avformat_open_input(&fmt_ctx, RTSP_URL, NULL, NULL) != 0) {
        perror("Can not open rtsp file.\n");
        exit(1);
    }
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) { 
        perror("Can not find stream info.\n");
        exit(1);
    }

    // av_dump_format(fmt_ctx, 0, RTSP_URL, 0); // 输出视频流详细信息
    av_init_packet(&av_packet);
    av_packet.data = NULL;
    av_packet.size = 0;

    int stream_idx = -1;
    // stream_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            stream_idx = i;
            printf("Find stream idx %d.\n", i);
            break;
        }
    }
    if (stream_idx < 0) {
        perror("Can not find stream in input file.\n");
        exit(1);
    }

    stream = fmt_ctx->streams[stream_idx];

    codec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (codec == NULL) {
        perror("Can not find decoder.\n");
        exit(1);
    }

    codec_ctx = avcodec_alloc_context3(NULL);
    if (!codec_ctx) {
        perror("Can not allocate video codec context.\n");
        exit(1);
    }

    if (avcodec_parameters_to_context(codec_ctx, stream->codecpar) < 0) {
        perror("Failed to copy codec parameters to decoder context.\n");
        exit(1);
    }

    avcodec_open2(codec_ctx, codec, NULL);

    // 初始化frame，解码数据
    frame = av_frame_alloc();
    if (!frame) {
        perror("Can nnot allocate video frame.\n");
        exit(1);
    }

    char buf[1024];

    socket_client.SendMsg("start");

    printf("Start to read frame.\n");
    // int i = 0;
    // system("rm -r test");
    // system("mkdir test");
    // while (true) { 
        while (av_read_frame(fmt_ctx, &av_packet) >= 0) {
            // if (av_packet.stream_index == stream_idx && (av_packet.flags & AV_PKT_FLAG_KEY)) { // 判断是否为关键帧
            if (av_packet.stream_index == stream_idx) { 
                if (avcodec_send_packet(codec_ctx, &av_packet) < 0) {
                    continue;
                }
                // 保存当前帧画面为图片
                while (avcodec_receive_frame(codec_ctx, frame) == 0) { // 一次可能不能接收到所有数据
                        SavePicture(frame);
                }

                // char tmp[100];
                // sprintf(tmp, "mv /data/documents/receive_from_cam_on_rk/cur_frame.jpg /data/documents/receive_from_cam_on_rk/test/%d.jpg", ++i);
                // system(tmp);

                // 进行人脸识别
                std::string cmp_res = CompareImageWithFaceLib(OUT_IMG_NAME);
                if (cmp_res.length() > 0) {
                    // time(&now);
                    // t = localtime(&now);
                    // printf("%d时%d分%d秒\n",t->tm_hour,t->tm_min,t->tm_sec);
                    // printf("persons: %s\n", cmp_res.data());
                    // 发送人脸识别结果
                    // printf("send*****************: %s\n", cmp_res.data());
                    socket_client.SendMsg(cmp_res.data());
                } 
            //     else { // FIXME:
                    // socket_client.SendMsg("pull");
            //     }
            }
        av_packet_unref(&av_packet);

        }
        // av_packet_unref(&av_packet);
    // }
    av_dict_free(&ops);
}


// 编码保存图片
void SavePicture(AVFrame *p_frame) {
    int width = p_frame->width;
    int height = p_frame->height;
    AVCodecContext *p_codec_ctx = NULL;

    AVFormatContext *p_fmt_ctx = avformat_alloc_context();
    // 设置输出文件格式
    p_fmt_ctx->oformat = av_guess_format("mjpeg", NULL, NULL);
    // 创建并初始化输出AVIOContext
    if (avio_open(&p_fmt_ctx->pb, OUT_IMG_NAME, AVIO_FLAG_READ_WRITE) < 0) {
        perror("Can not open output file.\n");
        exit(1);
    }
    // 构建一个新stream
    AVStream *p_av_stream = avformat_new_stream(p_fmt_ctx, 0);
    if (p_av_stream == NULL) {
        exit(1);
    }
    AVCodecParameters *params = p_av_stream->codecpar;
    params->codec_id = p_fmt_ctx->oformat->video_codec;
    params->codec_type = AVMEDIA_TYPE_VIDEO;
    params->format = AV_PIX_FMT_YUVJ420P;
    params->width = width;
    params->height = height;

    AVCodec *p_codec = (AVCodec *)avcodec_find_encoder(p_av_stream->codecpar->codec_id);
    if (!p_codec) {
        perror("Can not find encoder.\n");
        exit(1);
    }
    p_codec_ctx = avcodec_alloc_context3(p_codec);
    if (!p_codec_ctx) {
        perror("Can not allocate video codec context.\n");
        exit(1);
    }
    if (avcodec_parameters_to_context(p_codec_ctx, p_av_stream->codecpar) < 0) {
        perror("Failed to copy codec parameters to decoder context.\n");
        exit(1);
    }
    p_codec_ctx->time_base = (AVRational){1, 25};
    if (avcodec_open2(p_codec_ctx, p_codec, NULL) < 0) {
        perror("Can not open codec.\n");
        exit(1);
    }
    if (avformat_write_header(p_fmt_ctx, NULL) < 0) {
        perror("Failed to write header.\n");
        exit(1);
    }
    int y_size = width * height;
    // 编码，给AVPacket分配足够大的空间
    AVPacket packet;
    av_new_packet(&packet, y_size * 3);
    // 编码数据
    if (avcodec_send_frame(p_codec_ctx, p_frame) < 0) {
        perror("Error in avcodec_send_frame().\n");
        exit(1);
    }
    // 得到编码后数据
    if (avcodec_receive_packet(p_codec_ctx, &packet) < 0) {
        perror("Error in avcodec_receive_packet().\n");
        exit(1);
    }
    if (av_write_frame(p_fmt_ctx, &packet) < 0) {
        perror("Error in av_write_frame().\n");
        exit(1);
    }
    av_packet_unref(&packet);
    av_write_trailer(p_fmt_ctx);
    avcodec_close(p_codec_ctx);
    avio_close(p_fmt_ctx->pb);
    avformat_free_context(p_fmt_ctx);
}
