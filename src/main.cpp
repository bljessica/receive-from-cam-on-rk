#include <thread>

#include "rtsp_lib.h"
#include "Decoder.h"

#include "rockface_ctl.h"
#include "read_camera.h"
#include "SocketClient.h"


MyDecoder decoder = MyDecoder();


static void _on_video_data(const void *data, size_t len) {
  decoder.Decode_h264((void *)data, len); // 解码h264数据
}


rtsp_event_handler_t evt_handler = {
    .on_video_data = _on_video_data,
};


int main(){  
    system("rm /data/raw/*");
	// 初始化人脸识别，读入人脸库
    InitRockface();
    printf("Rockface initialized.\n");
    GeneratePersonNameMap();
    printf("Person name map generated.\n");
    std::vector<std::string> face_lib_files = GetPathJPGFiles(FACE_LIB_DATA_PATH);
    LoadFaceLibrary(face_lib_files);
    printf("Face lib loaded.\n");

    // 建立socket连接
    SocketClient socket_client = SocketClient();
    socket_client.Connect();
    printf("Socket connected.\n");

	// 人脸识别
    thread face_recognition_thread(ReadImg, socket_client);
    face_recognition_thread.detach();

    // 模糊图片
    thread blur_imgs_thread(BlurImgs);
    blur_imgs_thread.detach();

	// 读摄像头rtsp流
    start_rtspclient("172.16.55.31", "/ISAPI/streaming/channels/103", evt_handler, "554", "", "admin", "123456jl");
   
    socket_client.Disconnet();
    ReleaseRockface();
    return 0;  
}  