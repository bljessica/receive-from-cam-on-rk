#include <string>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include  <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

// #include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
// #include "jpeglib.h"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavformat/version.h>
    #include <libavutil/time.h>
    #include <libavutil/mathematics.h>
}

#include "SocketClient.h"
#include "rockface/rockface.h"



#define WORD uint16_t
#define DWORD uint32_t
#define LONG int32_t

#define RTSP_URL "rtsp://admin:123456jl@172.16.55.31:554"
#define OUT_IMG_PATH "./cur_frame.jpg"
#define IMG_WIDTH 704
#define IMG_HEIGHT 576
#define IMG_DIR_PATH "/data/raw/"
#define IMG_SUFFIX ".raw"

typedef unsigned char  BYTE;	// 定义BYTE类型，占1个字节

#pragma pack(2) // 节对齐方式为2字节对齐
 typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

using namespace std;
// using namespace cv;

// void ReadImg(char* imgName, SocketClient socket_client);
void ReadImg(SocketClient socket_client);
string GetNewestRawImgPath();
int GetRawImgNum();
// string GetNewestImgPath();
void ReadYUV(string inputFileName, rockface_image_t* img);
void ReadRtsp(SocketClient socket_client);
void saveBmp(AVFrame *avFrame, char *imgName, SocketClient socket_client);
int decodeWriteFrame(AVPacket *avPacket, AVFrame* avFrame, int *frameCount, int last, SocketClient socket_client) ;
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