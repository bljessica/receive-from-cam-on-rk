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

#include "rockface/rockface.h"

#include "SocketClient.h"
#include "rockface_ctl.h"


// #define IMG_WIDTH 1280
// #define IMG_HEIGHT 720
#define IMG_WIDTH 704
#define IMG_HEIGHT 576
#define DELETE_IMGS_REGULARLY_FLAG 0
#define IMG_DIR_PATH "/data/raw/"


using namespace std;

void ReadImg(SocketClient socket_client);
void BlurImgs();
void AdjustInterestBox(int frame_width, int frame_height);
void BlurArea(cv::Mat frame, cv::Rect area);
void BlurUninterestedArea(string img_path, cv::Rect box);
string GetNewestRawImgPath(int start_idx = 1);
int GetRawImgNum();
