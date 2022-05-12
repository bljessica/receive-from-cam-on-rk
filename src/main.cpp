// #include "SocketServer.h"
#include "rockface_control.h"
#include "read_camera.h"
#include "SocketClient.h"


int main(){  
    InitRockface();
    GeneratePersonNameMap();
    std::vector<std::string> face_lib_files = GetPathJPGFiles(FACE_LIB_DATA_PATH);

    LoadFaceLibrary(face_lib_files);
    printf("Face lib loaded.\n");

    // 建立socket连接
    SocketClient socket_client = SocketClient();
    socket_client.Connect();
    printf("Socket connected.\n");

    printf("Start to read rtsp.\n");
    ReadRtsp(socket_client);

    socket_client.Disconnet();
    ReleaseRockface();
    return 0;  
 }  


