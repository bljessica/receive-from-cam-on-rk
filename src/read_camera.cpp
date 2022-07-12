#include "read_camera.h"
#include "rockface_ctl.h"

// static AVFormatContext *formatContext = NULL;
// static AVCodecContext *codecCtx = NULL;
// struct SwsContext *swsContext = NULL;
// static char errors[200] = { 0 };
// const char *outName = "/data/raw";

// void ReadRtsp(SocketClient socket_client) {
//     // const char* srcMedia = "rtsp://admin:123456jl@172.16.55.31:554";
//     const char* srcMedia = "rtsp://admin:123456jl@172.16.55.31:554/ISAPI/streaming/channels/103";
//     //
//     av_log_set_level(AV_LOG_INFO);
//     av_register_all();
//     //获取输入上下文
//     int ret = avformat_open_input(&formatContext, srcMedia, NULL, NULL);
//     if (ret != 0) {
//         av_strerror(ret, errors, 200);
//         av_log(NULL, AV_LOG_WARNING, "avformat_open_input error: ret=%d, msg=%s\n", ret, errors);
//         exit(1);
//     }
//     ret = avformat_find_stream_info(formatContext, NULL);
//     if (ret != 0) {
//         av_strerror(ret, errors, 200);
//         av_log(NULL, AV_LOG_WARNING, "avformat_find_stream_info error: ret=%d, msg=%s\n", ret, errors);
//         exit(1);
//     }
//     av_dump_format(formatContext, 0, srcMedia, 0);
//     printf("===================================\n");
//     int inViIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
//     AVStream *inViStream = formatContext->streams[inViIndex];
//     //获取输入编解码器
//     AVCodec *codec = avcodec_find_decoder(inViStream->codec->codec_id);
//     codecCtx = avcodec_alloc_context3(NULL);
//     //获取输入编解码器上下文
//     ret = avcodec_parameters_to_context(codecCtx, inViStream->codecpar);
//     ret = avcodec_open2(codecCtx, codec, NULL);
//     //获取sws上下文
//     swsContext = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
//     codecCtx->width, codecCtx->height, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);
//     AVPacket avPacket;
//     av_init_packet(&avPacket);
//     int frameCount = 1;
//     AVFrame *avFrame = av_frame_alloc();
//     //获取packet
//     while (av_read_frame(formatContext, &avPacket) >= 0) {
//         if (avPacket.stream_index == inViIndex) {
//             //获取视频帧
//             decodeWriteFrame(&avPacket, avFrame, &frameCount, 0, socket_client);
//         }
//         av_packet_unref(&avPacket);
//         usleep(200);
//     }
//     decodeWriteFrame(&avPacket ,avFrame , &frameCount, 1, socket_client);
//     //
//     sws_freeContext(swsContext);
//     avcodec_free_context(&codecCtx);
//     avformat_close_input(&formatContext);
// }


// void saveBmp(AVFrame *avFrame, char *imgName, SocketClient socket_client) {
//     int w = avFrame->width;
//     int h = avFrame->height;
//     int size = avpicture_get_size(AV_PIX_FMT_BGR24, w, h);
//     uint8_t *buffer = (uint8_t*)av_malloc(size * sizeof(uint8_t));
//     AVFrame *frameRgb = av_frame_alloc();
//     avpicture_fill((AVPicture*)frameRgb, buffer, AV_PIX_FMT_BGR24, w, h);
//     sws_scale(swsContext, avFrame->data, avFrame->linesize, 0, h, frameRgb->data, frameRgb->linesize);
//     //2 构造 BITMAPINFOHEADER
//     BITMAPINFOHEADER header;
//     header.biSize = sizeof(BITMAPINFOHEADER);
//     header.biWidth = w;
//     header.biHeight = h * (-1);
//     header.biBitCount = 24;
//     header.biCompression = 0;
//     header.biSizeImage = 0;
//     header.biClrImportant = 0;
//     header.biClrUsed = 0;
//     header.biXPelsPerMeter = 0;
//     header.biYPelsPerMeter = 0;
//     header.biPlanes = 1;
//     //3 构造文件头
//     BITMAPFILEHEADER bmpFileHeader = { 0, };
//     //HANDLE hFile = NULL;
//     DWORD dwTotalWriten = 0;
//     //DWORD dwWriten;
//     bmpFileHeader.bfType = 0x4d42; //'BM';
//     bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;
//     bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
//     FILE *pFile = fopen(imgName, "wb");
//     fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
//     fwrite(&header, sizeof(BITMAPINFOHEADER), 1, pFile);
//     fwrite(frameRgb->data[0], 1, size, pFile);
//     fclose(pFile);
//     av_freep(&frameRgb);
//     av_free(frameRgb);
//     av_free(buffer);
//     // ReadImg(imgName, socket_client);
// }


// void ReadImg(char* imgName, SocketClient socket_client) {
//     // printf("last: %s, new: %s\n", last_img_path.c_str(), img_path.c_str());
//     printf("Recog************%s\n", imgName);
//     // 进行人脸识别
//     std::string cmp_res = CompareImageWithFaceLib(imgName);
//     if (cmp_res.length() > 0) {
//         // 发送人脸识别结果
//         printf("send*****************: %s\n", cmp_res.data());
//         socket_client.SendMsg(cmp_res.data());
//     }
// }

// int decodeWriteFrame(AVPacket *avPacket, AVFrame* avFrame, int *frameCount, int last, SocketClient socket_client) {
//     int getFrame = 0;
//     char buffer[200] = {0};
//     //解码
//     int len = avcodec_decode_video2(codecCtx, avFrame, &getFrame, avPacket);
//     if (len < 0) {
//         av_strerror(len, errors, 200);
//         av_log(NULL, AV_LOG_WARNING, "avcodec_decode_video2 error: ret=%d, msg=%s, frame=%d\n", len, errors, frameCount);
//         return len;
//     }
//     //保存bmp
//     if (getFrame) {
//         fflush(stdout);
//         snprintf(buffer, 200, "%s/%d.bmp", outName, *frameCount);
//         saveBmp(avFrame, buffer, socket_client);
//         (*frameCount)++;
//     }
//     return 0;
// }


// 读取rtsp流
void ReadImg(SocketClient socket_client) {
    char buf[1024];
    string last_img_path = ""; // 记录上一次识别的图片索引
    while (true) {
        if (GetRawImgNum() == 0) continue;
        string img_path = GetNewestRawImgPath();
        // printf("last: %s, new: %s\n", last_img_path.c_str(), img_path.c_str());
        if (strcmp(last_img_path.c_str(), img_path.c_str()) != 0) {
            printf("Recog************%s, %s\n", img_path.c_str(), (char*)img_path.c_str());
            // 进行人脸识别
            std::string cmp_res = CompareImageWithFaceLib((char*)img_path.c_str());
            if (cmp_res.length() > 0) {
                // 发送人脸识别结果
                printf("Send*****************: %s\n", cmp_res.data());
                socket_client.SendMsg(cmp_res.data());
            } 
            last_img_path = img_path;
        }
        usleep(200);
    }
}


void ReadYUV(string inputFileName, rockface_image_t* img) {
	int iImageSize = iImageSize = IMG_WIDTH * IMG_HEIGHT * 3 / 2;
	
	FILE * fpln;
	fpln = fopen(inputFileName.data(), "rb+");
	if (fpln == NULL) {
		printf("read yuv error.\n");
	}
 
	cv::Mat yuvImg;
	cv::Mat rgbImg(IMG_HEIGHT, IMG_WIDTH, CV_8UC3);
	yuvImg.create(IMG_HEIGHT * 3 / 2, IMG_WIDTH, CV_8UC1);
	
	unsigned char *pYUVbuf = new unsigned char[iImageSize];
	fread(pYUVbuf, iImageSize * sizeof(unsigned char), 1, fpln);
    fclose(fpln);
	memcpy(yuvImg.data, pYUVbuf, iImageSize * sizeof(unsigned char));
	cv::cvtColor(yuvImg, rgbImg, CV_YUV2RGBA_NV12);

    // img->data = (uint8_t*)malloc(rgbImg.total()*rgbImg.elemSize());
	// if(!img->data) {
    //     printf("no data in image\n");
    //     return;
    // }
	// memcpy(img->data, rgbImg.data, rgbImg.total()*rgbImg.elemSize());
    // img->height = rgbImg.rows;
    // img->width = rgbImg.cols;
    // img->size = rgbImg.total()*rgbImg.elemSize();//使用tatal()返回数组元素（如果该数组表示图像的像素数）的数目和每个像素占的字节
    // img->pixel_format = rgbImg.type()==CV_8UC3?ROCKFACE_PIXEL_FORMAT_BGR888:ROCKFACE_PIXEL_FORMAT_GRAY8;
    // img->is_prealloc_buf = 1;
    // img->original_ratio = ((float)img->width)/img->height;
 
	cv::imwrite(OUT_IMG_PATH, rgbImg);
}


// string GetNewestImgName() {
//     DIR *p_dir;
//     struct dirent *ptr;
//     if (!(p_dir = opendir(IMG_DIR_PATH))) {
//         printf("Can not open img dir path.\n");
//         return 0;
//     }
//     string newest_img_path;
//     while ((ptr = readdir(p_dir)) != 0) {
//         newest_img_path = ptr->d_name;
//         printf("cur newest_img_path: %s\n", newest_img_path.data());
//     }
//     closedir(p_dir);
//     return newest_img_path;
// }


int GetRawImgNum() {
    int file_num = 0;
    DIR *p_dir;
    struct dirent *ptr;
    if (!(p_dir = opendir(IMG_DIR_PATH))) {
        printf("Can not open img dir path.\n");
        return 0;
    }
    while ((ptr = readdir(p_dir)) != 0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
            file_num++;
        }
    }
    closedir(p_dir);
    return file_num;
}



string GetNewestRawImgPath() {
    // int img_num = GetRawImgNum();
    // string num_str = to_string(img_num - 2);
    // int padding_zero_num = 4 - num_str.length();
    // for (int i = 0; i < padding_zero_num; i++) {
    //     num_str = "0" + num_str;
    // }
    // string full_path = IMG_DIR_PATH + num_str + IMG_SUFFIX;
    // return full_path;
    int img_num = GetRawImgNum();
    string num_str = to_string(img_num - 1);
    string full_path = IMG_DIR_PATH + num_str + ".jpg";
    return full_path;
}



// // 读取rtsp流
// void ReadRtsp(SocketClient socket_client) {
//     AVFormatContext *fmt_ctx = NULL;
//     const AVCodec *codec;
//     AVCodecContext *codec_ctx = NULL;
//     AVStream *stream = NULL;
//     AVPacket av_packet;
//     AVFrame *frame;

//     AVDictionary *ops = NULL;
//     // 强制使用tcp，udp在1080p时会丢包导致花屏
//     av_dict_set(&ops, "rtsp_transport", "tcp", 0); // tcp更可靠但速度更慢
//     av_dict_set(&ops, "max_delay", "5000000", 0); // 设置最大时延5s
//     av_dict_set(&ops, "strict", "1", 0); // 使用更高的重排序计数
//     av_dict_set(&ops, "buffer_size", "1024000", 0); // 设置最大缓冲
//     av_dict_set(&ops, "probesize", "2048", 0); 
//     av_dict_set(&ops, "max_analyze_duration", "1000", 0); 
    
//     // av_dict_set(&ops, "max_delay", "500000", 0); 
//     // av_dict_set(&ops, "framerate", "20", 0); // 码率
//     // 打开网络流
//     if (avformat_open_input(&fmt_ctx, RTSP_URL, NULL, &ops) != 0) {
//     // if (avformat_open_input(&fmt_ctx, RTSP_URL, NULL, NULL) != 0) {
//         perror("Can not open rtsp file.\n");
//         exit(1);
//     }
//     if (avformat_find_stream_info(fmt_ctx, NULL) < 0) { 
//         perror("Can not find stream info.\n");
//         exit(1);
//     }

//     // av_dump_format(fmt_ctx, 0, RTSP_URL, 0); // 输出视频流详细信息
//     av_init_packet(&av_packet);
//     av_packet.data = NULL;
//     av_packet.size = 0;

//     int stream_idx = -1;
//     // stream_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
//     for (int i = 0; i < fmt_ctx->nb_streams; i++) {
//         if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
//             stream_idx = i;
//             printf("Find stream idx %d.\n", i);
//             break;
//         }
//     }
//     if (stream_idx < 0) {
//         perror("Can not find stream in input file.\n");
//         exit(1);
//     }

//     stream = fmt_ctx->streams[stream_idx];

//     codec = avcodec_find_decoder(stream->codecpar->codec_id);
//     if (codec == NULL) {
//         perror("Can not find decoder.\n");
//         exit(1);
//     }

//     codec_ctx = avcodec_alloc_context3(NULL);
//     if (!codec_ctx) {
//         perror("Can not allocate video codec context.\n");
//         exit(1);
//     }

//     if (avcodec_parameters_to_context(codec_ctx, stream->codecpar) < 0) {
//         perror("Failed to copy codec parameters to decoder context.\n");
//         exit(1);
//     }

//     avcodec_open2(codec_ctx, codec, NULL);

//     // 初始化frame，解码数据
//     frame = av_frame_alloc();
//     if (!frame) {
//         perror("Can nnot allocate video frame.\n");
//         exit(1);
//     }

//     char buf[1024];

//     socket_client.SendMsg("start");

//     printf("Start to read frame.\n");
//     // int i = 0;
//     // system("rm -r test");
//     // system("mkdir test");
//     // while (true) { 
//         while (av_read_frame(fmt_ctx, &av_packet) >= 0) {
//             // if (av_packet.stream_index == stream_idx && (av_packet.flags & AV_PKT_FLAG_KEY)) { // 判断是否为关键帧
//             if (av_packet.stream_index == stream_idx) { 
//                 if (avcodec_send_packet(codec_ctx, &av_packet) < 0) {
//                     continue;
//                 }
//                 // 保存当前帧画面为图片
//                 while (avcodec_receive_frame(codec_ctx, frame) == 0) { // 一次可能不能接收到所有数据
//                         SavePicture(frame);
//                 }

//                 // char tmp[100];
//                 // sprintf(tmp, "mv /data/documents/receive_from_cam_on_rk/cur_frame.jpg /data/documents/receive_from_cam_on_rk/test/%d.jpg", ++i);
//                 // system(tmp);

//                 // 进行人脸识别
//                 std::string cmp_res = CompareImageWithFaceLib(OUT_IMG_NAME);
//                 if (cmp_res.length() > 0) {
//                     // time(&now);
//                     // t = localtime(&now);
//                     // printf("%d时%d分%d秒\n",t->tm_hour,t->tm_min,t->tm_sec);
//                     // printf("persons: %s\n", cmp_res.data());
//                     // 发送人脸识别结果
//                     // printf("send*****************: %s\n", cmp_res.data());
//                     socket_client.SendMsg(cmp_res.data());
//                 } 
//             //     else { // FIXME:
//                     // socket_client.SendMsg("pull");
//             //     }
//             }
//         av_packet_unref(&av_packet);

//         }
//         // av_packet_unref(&av_packet);
//     // }
//     av_dict_free(&ops);
// }


// // 编码保存图片
// void SavePicture(AVFrame *p_frame) {
//     int width = p_frame->width;
//     int height = p_frame->height;
//     AVCodecContext *p_codec_ctx = NULL;

//     AVFormatContext *p_fmt_ctx = avformat_alloc_context();
//     // 设置输出文件格式
//     p_fmt_ctx->oformat = av_guess_format("mjpeg", NULL, NULL);
//     // 创建并初始化输出AVIOContext
//     if (avio_open(&p_fmt_ctx->pb, OUT_IMG_NAME, AVIO_FLAG_READ_WRITE) < 0) {
//         perror("Can not open output file.\n");
//         exit(1);
//     }
//     // 构建一个新stream
//     AVStream *p_av_stream = avformat_new_stream(p_fmt_ctx, 0);
//     if (p_av_stream == NULL) {
//         exit(1);
//     }
//     AVCodecParameters *params = p_av_stream->codecpar;
//     params->codec_id = p_fmt_ctx->oformat->video_codec;
//     params->codec_type = AVMEDIA_TYPE_VIDEO;
//     params->format = AV_PIX_FMT_YUVJ420P;
//     params->width = width;
//     params->height = height;

//     AVCodec *p_codec = (AVCodec *)avcodec_find_encoder(p_av_stream->codecpar->codec_id);
//     if (!p_codec) {
//         perror("Can not find encoder.\n");
//         exit(1);
//     }
//     p_codec_ctx = avcodec_alloc_context3(p_codec);
//     if (!p_codec_ctx) {
//         perror("Can not allocate video codec context.\n");
//         exit(1);
//     }
//     if (avcodec_parameters_to_context(p_codec_ctx, p_av_stream->codecpar) < 0) {
//         perror("Failed to copy codec parameters to decoder context.\n");
//         exit(1);
//     }
//     p_codec_ctx->time_base = (AVRational){1, 25};
//     if (avcodec_open2(p_codec_ctx, p_codec, NULL) < 0) {
//         perror("Can not open codec.\n");
//         exit(1);
//     }
//     if (avformat_write_header(p_fmt_ctx, NULL) < 0) {
//         perror("Failed to write header.\n");
//         exit(1);
//     }
//     int y_size = width * height;
//     // 编码，给AVPacket分配足够大的空间
//     AVPacket packet;
//     av_new_packet(&packet, y_size * 3);
//     // 编码数据
//     if (avcodec_send_frame(p_codec_ctx, p_frame) < 0) {
//         perror("Error in avcodec_send_frame().\n");
//         exit(1);
//     }
//     // 得到编码后数据
//     if (avcodec_receive_packet(p_codec_ctx, &packet) < 0) {
//         perror("Error in avcodec_receive_packet().\n");
//         exit(1);
//     }
//     if (av_write_frame(p_fmt_ctx, &packet) < 0) {
//         perror("Error in av_write_frame().\n");
//         exit(1);
//     }
//     av_packet_unref(&packet);
//     av_write_trailer(p_fmt_ctx);
//     avcodec_close(p_codec_ctx);
//     avio_close(p_fmt_ctx->pb);
//     avformat_free_context(p_fmt_ctx);
// }
