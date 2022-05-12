#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define MAX_BUFFER_SIZE 1500
#define SERVER_IP "172.16.55.49"
#define SERVER_PORT 9987

#pragma once
class SocketClient {
    public:
        SocketClient();
        void Connect();
        void SendMsg(const char *msg);
        void Disconnet();
    private:
        struct sockaddr_in _server_addr, _client_addr; // 服务端、客户端地址
        char buffer[MAX_BUFFER_SIZE]; // 缓冲区
        int _client_socket;
        int _addr_len;
        FILE *_fp;
        int _file_len;;
};
