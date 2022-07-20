#include "rockface_ctl.h"


rockface_ret_t ret;
rockface_handle_t face_handle;

std::map<string, rockface_feature_t> img_feature_map; // 图片名-人脸特征map
std::map<string, string> img_person_map; // 图片名-人名map
char img_names[200][50];
char person_names[200][20];


// 初始化rockface
void InitRockface() {
    // 创建rockface句柄
    face_handle = rockface_create_handle();
    // 设置license
    ret = rockface_set_licence(face_handle, LICENSE_PATH);
    if (ret != ROCKFACE_RET_SUCCESS) {
        printf("Rockface authorization error %d!\n", ret);
        exit(1);
    }
    // 设置数据文件在设备的路径
    rockface_set_data_path(face_handle, FACE_LIB_DATA_PATH);
    // 初始化
    ret = rockface_init_detector(face_handle);
    if (ret != ROCKFACE_RET_SUCCESS) {
        printf("Init detector error %d!\n", ret);
        exit(1);
    }
    ret = rockface_init_landmark(face_handle, 5);
    if (ret != ROCKFACE_RET_SUCCESS) {
        printf("Init landmark error %d!\n", ret);
        exit(1);
    }
    ret = rockface_init_recognizer(face_handle);
    if (ret != ROCKFACE_RET_SUCCESS) {
        printf("Init recognizer error %d!", ret);
        exit(1);
    }
}


// 获取路径下所有jpg图片路径
std::vector<string> GetPathJPGFiles(string path) {
	std::vector<string> files;
    DIR *dir;
	struct dirent *ptr;
	if ((dir = opendir(path.c_str())) == NULL) {
		perror("Open dir error.\n");
		exit(1);
	}
	while ((ptr = readdir(dir)) != NULL) {
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) // 当前文件夹或父文件夹
			continue;
		else if (ptr->d_type == 8) { // 常规文件
            if (strstr(ptr->d_name, ".jpg") != NULL) {
			    files.push_back(path + "/" + ptr->d_name);
            }
        } 
		else if (ptr->d_type == 10) { // 符号链接
			continue;
        }
		else if (ptr->d_type == 4) { // 目录
			files.push_back(path + "/" + ptr->d_name);
        }
	}
	closedir(dir);
    if (files.size() < 1) {
        printf("No face image, path: %s\n", path.c_str());
        exit(1);
    } else {
        printf("Image count: %d\n", (int)files.size());
    }
	return files;
}


// 调整人像矩形坐标（返回: true 有效值  false 无效值）
bool AdjustROIPos(rockface_rect_t *box, rockface_image_t *img) {
    // printf("Before adjust: top: %d, left: %d, bottom: %d, right: %d\n", box->top, box->left, box->bottom, box->right);
    if (box->top < 0) {
        box->top = 0;
    }
    if (box->left < 0) {
        box->left = 0;
    }
    if (box->right >= img->width) {
        box->right = img->width - 1;
    }
    if (box->bottom >= img->height) {
        box->bottom = img->height - 1;
    }
    // printf("Adjust: top: %d, left: %d, bottom: %d, right: %d\n", box->top, box->left, box->bottom, box->right);
    if (box->top >= box->bottom || box->left >= box->right) {
        // printf("************************error************************\n");
        return false;
    }
    return true;
}


// 读取人脸库图片，进行特征提取
void LoadFaceLibrary(std::vector<string> image_files) {
    for (int i = 0; i < image_files.size(); i++) {
        string img_path = image_files[i];
        rockface_image_t img;
        if (!rockface_image_read(img_path.c_str(), &img, 1)) { // 读取图像文件成功
            rockface_det_array_t face_array;
            memset(&face_array, 0, sizeof(rockface_det_array_t));
            if (!rockface_detect(face_handle, &img, &face_array)) { // 人脸检测成功
                if ((&face_array)->count == 1) { // 人脸库照片识别出一张人脸
                    rockface_image_t out_img; // 对齐后的人脸图像
                    if (AdjustROIPos(&((&face_array)->face[0]).box, &img)) {
                        if (!rockface_align(face_handle, &img, &((&face_array)->face[0]).box, NULL, &out_img)) { // 人脸关键点（5点）检测，矫正对齐成功
                            rockface_feature_t out_feature; // 人脸特征
                            if (!rockface_feature_extract(face_handle, &out_img, &out_feature)) { // 对已对齐的人脸图像提取人脸特征成功
                                int img_name_pos = img_path.find_last_of('/'); // 图片名在图片路径字符串中的位置
                                string img_name(img_path.substr(img_name_pos + 1)); // 图片名
                                img_feature_map[img_name] = out_feature;
                            }
                        }
                    }
                    rockface_image_release(&out_img);
                }
            }
        }   
        rockface_image_release(&img);
    }
}


// 将提取的人脸特征与人脸库进行比对
string CompareImageWithFaceLib(char* img_name) {
    string person_names_str = "";
    rockface_image_t img;
    rockface_image_read(img_name, &img, 1); // 读取图像文件成功
    rockface_det_array_t face_array;
    memset(&face_array, 0, sizeof(rockface_det_array_t));
    if (!rockface_detect(face_handle, &img, &face_array)) { // 人脸检测成功
        int count = (&face_array)->count;
        printf("detect count: %d\n", count);
        for (int i = 0; i < count; i++) {
            rockface_image_t out_img; // 对齐后的人脸图像
            rockface_rect_t* box = &((&face_array)->face[i]).box;
            if (AdjustROIPos(box, &img)) {
                if (!rockface_align(face_handle, &img, box, NULL, &out_img)) { // 人脸关键点（5点）检测，矫正对齐成功
                    rockface_feature_t out_feature; // 人脸特征
                    if (!rockface_feature_extract(face_handle, &out_img, &out_feature)) { // 对已对齐的人脸图像提取人脸特征成功
                        string cmp_res = CompareFeatureWithFaceLib(out_feature);
                        if (cmp_res.length() > 0) {
                            // 拼接识别信息
                            string tmp = cmp_res + "|" + to_string(box->left) + "," + to_string(box->top) + "," + to_string(box->right - box->left)
                                + "," + to_string(box->bottom - box->top) + ";";
                            printf("person info: %s\n", tmp.data());
                            person_names_str += tmp;
                        }
                    }
                }
            }
            rockface_image_release(&out_img);
        }
    }
    rockface_image_release(&img);
    memset(&face_array, 0, sizeof(rockface_det_array_t));
    return person_names_str;
}


// 将特征与人脸库特征作比对，返回人名
string CompareFeatureWithFaceLib(rockface_feature_t feature) {
    float max_similarity = -1;
    string max_similarity_person_name = "";
    for(auto it:img_feature_map) {
        float out_similarity;
        if (!rockface_feature_compare(&feature, &(it.second), &out_similarity)) {
            if (out_similarity > SIMILARITY_THRESHOLD && out_similarity > max_similarity) {
                max_similarity = out_similarity;
                max_similarity_person_name = img_person_map[it.first.data()];
            }
            // printf("img_name: %s, person_name: %s, similarity: %f\n", it.first.data(), img_person_map[it.first.data()].data(), out_similarity);
        }
    }
    if (max_similarity_person_name.length() > 0) {
        printf("similarity: %f\n", max_similarity);
    }
    return max_similarity_person_name;
}


// 读取配置文件，生成图片名-人名map
void GeneratePersonNameMap() {
    // 打开文件
    FILE *fp;
    fp = fopen(FACE_LIB_PERSON_JSON_PATH, "r");
    if (fp == NULL) {
        printf("Open json file error.\n");
        fclose(fp);
        exit(1);
    }
    // 获得文件大小
    struct stat stat_buffer;
    stat(FACE_LIB_PERSON_JSON_PATH, &stat_buffer);
    int file_size = stat_buffer.st_size;
    printf("Json file size: %d.\n", file_size);
    // 分配内存
    char *json_str = (char *)malloc(sizeof(char) * file_size + 1);
    memset(json_str, 0, file_size + 1);
    // 读取json文件
    int size = fread(json_str, sizeof(char), file_size, fp);
    if (size == 0) {
        printf("Read json file error.\n");
        exit(1);
    }
    fclose(fp);

    // 将读取到的json字符串转换成cJson指针
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        free(json_str);
        exit(1);
    }
    free(json_str);

    cJSON *staffs = cJSON_GetObjectItem(root, "staffs");
    if (staffs != NULL) {
        int staff_num = cJSON_GetArraySize(staffs); // 获取数组大小
        for (int i = 0; i < staff_num; i++) {
            cJSON *staff = cJSON_GetArrayItem(staffs, i); // 获取单个员工数据
            if (staff != NULL && staff->type == cJSON_Object) {
                cJSON *person_name_val = cJSON_GetObjectItem(staff, "empname");
                if (person_name_val != NULL && person_name_val->type == cJSON_String) {
                    char *person_name = cJSON_Print(person_name_val);
                    cJSON *cards = cJSON_GetObjectItem(staff, "cards");
                    int card_num = cJSON_GetArraySize(cards);
                    cJSON *last_card = cJSON_GetArrayItem(cards, card_num - 1);
                    cJSON *img_name_val = cJSON_GetObjectItem(last_card, "mame");
                    if (img_name_val != NULL && img_name_val->type == cJSON_String) {
                        char *img_name = cJSON_Print(img_name_val);
                        string t1 = img_name;
                        string t2 = person_name;
                        img_person_map[t1.substr(1, t1.length() - 2)] = t2.substr(1, t2.length() - 2); // 去掉首尾双引号
                    }
                }
            }
        }
    }
    cJSON_Delete(root); //释放
}


// 打印img_person_map（not in use）
void PrintPersonNameMap() {
    for(auto it:img_person_map) {
        printf("img_name: %s, person_name:%s \n", it.first.data(), it.second.data());
        // printf("img_name: %s, person_name:%s, %s, %s\n", it.first.data(), img_person_map[it.first], img_person_map[it.first.data()].data(), it.second.data());
    }
    // map<string, string>::iterator it;
    //  for(it = img_person_map.begin(); it != img_person_map.end(); it++) {
    //      printf("%s, %s, %s\n", it->first.data(), it->second.data(), img_person_map[it->first.data()].data());
    // }
}


// 释放rockface句柄
void ReleaseRockface() {
    rockface_release_handle(face_handle);
}