#include "read_camera.h"


cv::Rect interest_box; // 图片感兴趣区域（不模糊）
bool blur_imgs_flag = false; // 是否模糊图片
string last_img_path = ""; // 记录上一次识别的图片索引


// 对最新的图片进行人脸识别，发送结果给服务端
void ReadImg(SocketClient socket_client) {
    char buf[1024];
    int num = 1; // 记录删除图片周期
    int img_idx = 1; // 读到的图片索引
    while (true) {
        bool is_img_send = false;
        if (GetRawImgNum() <= 1) continue;
        string img_path = GetNewestRawImgPath(img_idx);
        // 判断图片是否存在
        try {
            cv::Mat img = cv::imread(img_path);
        } catch (...) { 
            printf("Caught error.\n");
            continue;
        }
        // printf("last: %s, new: %s\n", last_img_path.c_str(), img_path.c_str());
        if (strcmp(last_img_path.c_str(), img_path.c_str()) != 0) {
            printf("Recog************%s, %s\n", img_path.c_str(), (char*)img_path.c_str());
            // 进行人脸识别
            std::string cmp_res = CompareImageWithFaceLib((char*)img_path.c_str());
            if (cmp_res.length() > 0) {
                // 发送人脸识别结果
                printf("Send*****************: %s\n", cmp_res.data());
                socket_client.SendMsg(cmp_res.data());
                is_img_send = true;
            } 
            last_img_path = img_path;
        }
        if (!is_img_send) {
            socket_client.SendMsg("");
        }
        if (strcmp(socket_client.rec_buffer, "finish") == 0) { // 停止扫描
            cout << "Finished recognition." << endl;
            // system("rm /data/raw/*"); // 删除文件夹下所有图片
            blur_imgs_flag = true;
            break;
        }
        num++;
        if (DELETE_IMGS_REGULARLY_FLAG && num > 100) { // 需要定期删除图片
            num -= 100;
            img_idx += GetRawImgNum();
            system("rm /data/raw/*"); // 删除文件夹下所有图片
            printf("Delete imgs.\n");
            // usleep(300); // 确保文件夹内有图片
        }
    }
}


// 模糊图片
void BlurImgs() {
    AdjustInterestBox(IMG_WIDTH, IMG_HEIGHT);
    int idx, img_start_idx;
    char img_path[200];
    bool start_blur = false;
    while (true) {
        usleep(100);
        if (blur_imgs_flag) {
            if (!start_blur) { // 刚开始模糊，确定开始模糊的图片索引
                // usleep(500); // 确保文件夹内有图片
                int num_start_idx = last_img_path.find_last_of("/");
                int num_end_idx = last_img_path.find_last_of(".");
                string num_str = last_img_path.substr(num_start_idx + 1, num_end_idx - num_start_idx);
                idx = atoi(num_str.c_str());
                // img_start_idx = idx; // 最开始模糊的图片索引
                start_blur = true;
            } else { // 开始模糊
                if (idx >= GetRawImgNum()) continue; // TODO: 定期删除图片情况
                // if (idx >= GetRawImgNum() + img_start_idx) continue;
                sprintf(img_path, "%s%d.jpg", IMG_DIR_PATH, idx++); // sprintf函数发送格式化输出到指定字符串
                BlurUninterestedArea(img_path, interest_box);
            }
        }
        
    }
}


// 设置感兴趣的框
void AdjustInterestBox(int frame_width, int frame_height) {
    // 图像中间部分
    interest_box = cv::Rect(frame_width * 3 / 8, (frame_height - frame_width / 4) / 2, frame_width / 4, frame_width / 4);
    // printf("After adjust: [%d %d %d %d]\n", interest_box.x, interest_box.y, interest_box.width, interest_box.height);
}


// 模糊区域
void BlurArea(cv::Mat frame, cv::Rect area) {
    // printf("rows: %d, cols: %d, x: %d, y: %d, width: %d, height: %d\n", frame.rows, frame.cols, area.x, area.y, area.width, area.height);
    cv::Mat rot(frame, area);
    cv::blur(rot, rot, cv::Size(50, 50), cv::Point(-1, -1));
}


// 模糊除指定区域以外的区域
void BlurUninterestedArea(string img_path, cv::Rect box) {
    cv::Mat img = cv::imread(img_path);
    // printf("***********************************************************************************blur: %s\n", img_path.c_str());
    if (!img.data) {
        printf("Can not read img: %s.\n", img_path.c_str());
        return;
    }
    // printf("BlurUninterestedArea: [%d %d %d %d]\n", box.x, box.y, box.width, box.height);
    if (box.y > 0) { // 上
        BlurArea(img, cv::Rect(0, 0, img.cols, box.y));
    }
    if (box.x > 0) { // 左
        BlurArea(img, cv::Rect(0, box.y, box.x, img.rows - box.y));
    }
    if (box.x + box.width < img.cols) { // 右
        BlurArea(img, cv::Rect(box.x + box.width, box.y, img.cols - box.x - box.width, img.rows - box.y));
    }
    if (box.y + box.height < img.rows) { // 下
        BlurArea(img, cv::Rect(box.x, box.y + box.height, box.width, img.rows - box.y - box.height));
    }
    cv::imwrite(img_path, img); // 重新写入图片
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
string GetNewestRawImgPath(int start_idx) {
    int img_num = GetRawImgNum();
    string num_str = to_string(start_idx + img_num - 2); // 确保图片存在
    string full_path = IMG_DIR_PATH + num_str + ".jpg";
    return full_path;
}
