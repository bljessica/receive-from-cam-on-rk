#include <pthread.h>
#include <thread>

#include "rockface_ctl.h"
#include "read_camera.h"
#include "SocketClient.h"
#include "camrgb_control.h"


int main(){  
    // system("rm ./imgs/*");
    system("rm /data/raw/*");
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

    thread face_recognition_thread(ReadImg, socket_client);
    face_recognition_thread.detach();

    ReadRtsp(socket_client);

    // pthread_t face_recognition_thread_id;
    // int ret = pthread_create(&face_recognition_thread_id, NULL, ReadImg, &socket_client)
    
    // camrgb_control_init(socket_client);
    
    // sleep(1000);

    // ReadImg(socket_client);

    socket_client.Disconnet();
    // camrgb_control_exit();
    ReleaseRockface();
    return 0;  
}  