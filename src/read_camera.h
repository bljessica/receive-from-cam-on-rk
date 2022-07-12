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


#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define IMG_DIR_PATH "/data/raw/"


using namespace std;

void ReadImg(SocketClient socket_client);
string GetNewestRawImgPath();
int GetRawImgNum();
