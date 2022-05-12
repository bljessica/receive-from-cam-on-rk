extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
#include <string>
// #include <time.h>

#include "SocketClient.h"


// #define RTSP_URL "rtsp://admin:jldz12345@192.168.5.15:554/h264/ch1/main/av_stream"
// #define RTSP_URL "rtsp://admin:123456jl@172.16.55.31:554/h264/ch1/main/av_stream"
#define RTSP_URL "rtsp://admin:123456jl@172.16.55.31:554"
// #define RTSP_URL "rtsp://admin:123456jl@172.16.55.31:554/h264/ch1/sub/av_stream "
#define OUT_IMG_NAME "cur_frame.jpg"


using namespace std;


void ReadRtsp(SocketClient socket_client);
void SavePicture(AVFrame *frame);