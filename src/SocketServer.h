#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>


#define MAX_BUFFER_SIZE 256 // 2073600 // 1080 * 1920
#define SERVER_PORT  9987
 

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
};
