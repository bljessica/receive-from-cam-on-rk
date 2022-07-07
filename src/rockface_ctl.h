#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <dirent.h>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <sys/stat.h>
#include "cJSON.h"

#include "rockface/rockface.h"


#define LICENSE_PATH "/data/rkfacial_data/key.lic"
#define FACE_LIB_DATA_PATH "/data/rkfacial_data/white_list"
#define FACE_LIB_PERSON_JSON_PATH "/data/rkfacial_data/white_list/wlist.json"
#define SIMILARITY_THRESHOLD 0.4 // 两个人脸特征的相似度 (使用欧式距离)，可以设置阈值(建议阈值范围0.7~1.3，可根据不同人脸库和应用场景调整)，如果小于阈值可以判断为同一人
#define IMG_WIDTH 704
#define IMG_HEIGHT 576

using namespace std;


void InitRockface();
std::vector<string> GetPathJPGFiles(string path);
bool AdjustROIPos(rockface_det_t face, rockface_image_t* img);
void LoadFaceLibrary(std::vector<string> image_files);
void YUV2RockfaceImage(string yuv_file_path, rockface_image_t *img);
string CompareImageWithFaceLib(rockface_image_t *img);
string CompareFeatureWithFaceLib(rockface_feature_t feature);
string CompareImageWithFaceLib(char* img_name);
void GeneratePersonNameMap();
void PrintPersonNameMap();
void ReleaseRockface();