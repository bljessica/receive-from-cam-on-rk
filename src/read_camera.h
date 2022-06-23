#include <string>
#include <errno.h>
#include <dirent.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
// #include "jpeglib.h"

#include "SocketClient.h"


#define RTSP_URL "rtsp://admin:123456jl@172.16.55.31:554"
#define OUT_IMG_PATH "./cur_frame.jpg"
#define IMG_WIDTH 1280
#define IMG_HEIGHT 720
#define IMG_DIR_PATH "/data/raw/"
#define IMG_SUFFIX ".raw"

typedef unsigned char  BYTE;	// 定义BYTE类型，占1个字节

using namespace std;
// using namespace cv;

void ReadImg(SocketClient socket_client);
string GetNewestImgName();
int GetImgNum();
string GetNewestImgPath();
void YUV2JPG(string inputFileName, string savepath);


// extern "C" {
// #include <libavcodec/avcodec.h>
// #include <libavformat/avformat.h>
// #include <libavutil/avutil.h>
// }
// #include <string>
// // #include <time.h>

// #include "SocketClient.h"
// #include "rtsp/Timestamp.h"


// // #define RTSP_URL "rtsp://admin:jldz12345@192.168.5.15:554/h264/ch1/main/av_stream"
// // #define RTSP_URL "rtsp://admin:123456jl@172.16.55.31:554/h264/ch1/main/av_stream"
// #define RTSP_URL "rtsp://admin:123456jl@172.16.55.31:554"
// // #define RTSP_URL "rtsp://admin:123456jl@172.16.55.31:554/h264/ch1/sub/av_stream "
// #define OUT_IMG_NAME "cur_frame.jpg"


// using namespace std;


// void ReadRtsp(SocketClient socket_client);
// void SavePicture(AVFrame *frame);

// #include <thread>

// #include "rtsp/RtspClient.h"
// #include "rtsp/display.h"
// #include "rtsp/rtsp_lib.h"
// #include "rkmedia/rkmedia_api.h"

// #define RTSP_URL "rtsp://admin:123456jl@172.16.55.31:554"

// void start_rtspclient(rtsp_event_handler_t evt_handler  ,bool flag  );
// void stop_rtspclient();
// void receiveVideoThread(xop::RtspClient* rtspClient);