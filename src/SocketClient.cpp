#include "SocketClient.h"


SocketClient::SocketClient() {

}


// 建立Socket连接
void SocketClient::Connect() {
    _client_socket = socket(AF_INET, SOCK_STREAM, 0); // 创建socket连接
    if (_client_socket < 0) { // 创建失败
        fprintf(stderr, "Socket error: %s\n", strerror(errno));
        Disconnet();
        exit(1);
    }
    // 初始化客户端地址
    _addr_len = sizeof(struct sockaddr_in);
    bzero(&_client_addr, _addr_len);
    _client_addr.sin_family = AF_INET;
    _client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    _client_addr.sin_port = 0;
    if (bind(_client_socket, (struct sockaddr*)&_client_addr, _addr_len) < 0) { // 绑定失败
        fprintf(stderr, "Bind error: %s\n", strerror(errno));
        Disconnet();
        exit(1);
    }
    // 初始化服务端地址
    _addr_len = sizeof(struct sockaddr_in);
    bzero(&_server_addr, _addr_len);
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    _server_addr.sin_port = htons(SERVER_PORT);
    if (connect(_client_socket, (struct sockaddr*)&_server_addr, _addr_len) != 0) { // 连接失败
        fprintf(stderr, "Connect error: %s\n", strerror(errno));
        Disconnet();
        exit(1);
    }
    printf("Connected with server successfully.\n");
}


// 收发字符串消息
void SocketClient::SendMsg(const char *msg) {
    strcpy(buffer, msg);  
    send(_client_socket, buffer, sizeof(buffer), 0); //发送数据
    printf("Sended.\n");
    // memset(buffer, 0, sizeof(buffer));
    // int num_read; // 接受数据 
    // num_read = recv(_client_socket, buffer, MAX_BUFFER_SIZE, 0);
    // printf("received\n\n");
    // return buffer;
}


// 断开连接
void SocketClient::Disconnet() {
    close(_client_socket); 
    printf("Socket disconnected.\n");
}