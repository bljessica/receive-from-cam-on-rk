#include "SocketServer.h"
#include "rockface_control.h"


SocketServer::SocketServer() {
    

}


// 创建socket连接
void SocketServer::Connect() {
    _server_socket = socket(AF_INET,SOCK_STREAM, 0);
    if(_server_socket < 0) { // 创建失败
        fprintf(stderr, "socker Error:%s\n", strerror(errno));  
        exit(1); 
    }
    // 初始化服务器地址
    socklen_t addr_len = sizeof(struct sockaddr_in);  
    bzero(&_server_addr, addr_len);  
    _server_addr.sin_family = AF_INET;  
    _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    _server_addr.sin_port = htons(SERVER_PORT);  
    // 设置套接字选项避免Address already in use错误
    int on = 1;
    if((setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))<0)
    {
        perror("Setsockopt failed");
        exit(1);
    }
    if(bind(_server_socket,(struct sockaddr*)&_server_addr, sizeof(struct sockaddr_in)) < 0){ // 绑定失败 
        fprintf(stderr, "Bind error: %s\n", strerror(errno));
        Disconnet(); 
        exit(1);  
    }  
    // 监听客户端连接请求，监听队列长度位5
    if(listen(_server_socket, 5) < 0) {  
        fprintf(stderr, "Listen error: %s\n", strerror(errno));
        Disconnet();
        exit(1);  
    }  
    printf("Start listening...\n");
    
    if ((_client_socket = accept(_server_socket, (struct sockaddr*) &_client_addr, &addr_len)) < 0) {
        perror("Aceept error.\n");
        Disconnet(); 
        exit(1);
    }
    printf("accept client %s\n", inet_ntoa(_client_addr.sin_addr)); 

    while (1) {
        int num_read = recv(_client_socket, _buffer, MAX_BUFFER_SIZE, 0);
        if (num_read != 0) { 
            if (strcmp(_buffer, "SENDED") == 0) {
                memset(_buffer, 0, sizeof(_buffer));
                // 进行人脸特征提取
                std::string cmp_res = CompareImageWithFaceLib(CUR_FRAME_IMAGE_FILE_NAME);
                if (cmp_res.length() > 0) {
                    printf("persons: %s\n", cmp_res.data());
                    sprintf(_buffer, "%s", cmp_res.data());
                    send(_client_socket, _buffer, sizeof(_buffer), 0);
                    continue;
                }
            } 
        }
        // 未识别到
        strcpy(_buffer, "None");
        send(_client_socket, _buffer, sizeof(_buffer), 0);
    }
}


// 接收客户端发送的图片并返回相应结果(Not in use)
void SocketServer::ReceiveImage() {
    // 接收客户端输入
    if ((_fp = fopen(CUR_FRAME_IMAGE_FILE_NAME, "wb")) == NULL) {
        perror("Open save file error.\n");
        exit(1);
    }
    // 分段接收并存储图片
    int num_read = 0;
    while (1) {
        num_read = recv(_client_socket, _buffer, MAX_BUFFER_SIZE, 0);
        if (num_read == 0) { // 已接收完
            break;
        }
        fwrite(_buffer, 1, MAX_BUFFER_SIZE, _fp);
        // printf("writing...\n");
    }
    // TODO: 进行人脸识别
    // FIXME: 模拟
    // if (_i % 3 == 0) { // 识别到了人脸
    //     sprintf(_buffer, "Deng %d", std::to_string(_i));
    //     // strcpy(_buffer, "Deng" + std::to_string(i));
    //     send(_client_socket, _buffer, sizeof(_buffer), 0);
    // } else { // 未识别到
    //     strcpy(_buffer, "None");
    //     send(_client_socket, _buffer, sizeof(_buffer), 0);
    // }
    printf("Image received.\n");
}


// 释放socket连接
void SocketServer::Disconnet() {
    close(_server_socket);  
    printf("Socket disconnected.\n");
}
