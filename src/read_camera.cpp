#include "read_camera.h"


// 对最新的图片进行人脸识别，发送结果给服务端
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


// 计算图片文件夹下的图片数量
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


// 得到最新的jpg图片
string GetNewestRawImgPath() {
    int img_num = GetRawImgNum();
    string num_str = to_string(img_num - 1);
    string full_path = IMG_DIR_PATH + num_str + ".jpg";
    return full_path;
}
