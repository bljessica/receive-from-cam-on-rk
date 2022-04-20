#include "SocketServer.h"
#include "rockface_control.h"


int main(){  
    InitRockface();
    GeneratePersonNameMap();
    // PrintPersonNameMap();
    std::vector<std::string> face_lib_files = GetPathJPGFiles(FACE_LIB_DATA_PATH);

    LoadFaceLibrary(face_lib_files);

    // string res = CompareImageWithFaceLib("cur_frame.jpg");
    // printf("res: %s \n", res.data());

    SocketServer socket_server = SocketServer();
    socket_server.Connect();


    socket_server.Disconnet();
    ReleaseRockface();
    return 0;  
 }  


