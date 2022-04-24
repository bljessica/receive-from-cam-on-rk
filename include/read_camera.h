extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
#include <string>


#define RTSP_URL "rtsp://admin:jldz12345@192.168.5.15:554/h264/ch1/main/av_stream"
#define OUT_IMG_NAME "cur_frame.jpg"
// #define OUT_IMG_TYPE ".jpg"


using namespace std;


void ReadRtsp();
void SavePicture(AVFrame *frame);