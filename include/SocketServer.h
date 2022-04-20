#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <string.h>


#define MAX_BUFFER_SIZE 1500 
#define SERVER_PORT  9987
#define CUR_FRAME_IMAGE_FILE_NAME "cur_frame.jpg"
 

class SocketServer {
    public:
        SocketServer();
        void Connect();
        void Disconnet();


    private:
        struct sockaddr_in _server_addr; // 本机地址信息 
        struct sockaddr_in _client_addr; // 客户端地址信息 
        char _buffer[MAX_BUFFER_SIZE]; // 缓冲区  
        int _server_socket; // 创建连接的socket
        int _client_socket; // client_fd：数据传输socket 
        FILE *_fp;
        const char *photos_dir = "/data/rkfacial_data/white_list"; // rk1109上人脸库路径

        void ReceiveImage();
};
