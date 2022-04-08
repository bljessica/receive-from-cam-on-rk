#include "SocketServer.h"


SocketServer::SocketServer() {
    
}


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
    if(bind(_server_socket,(struct sockaddr*)&_server_addr, sizeof(struct sockaddr_in)) < 0){ // 绑定失败 
        perror("Bind Error:%s\n");  
        Disconnet(); 
        exit(1);  
    }  
    // 监听客户端连接请求，监听队列长度位5
    if(listen(_server_socket, 5) < 0) {  
        perror("Listen Error:%s\n");  
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
    // strcpy(_buffer, "Hi, I am server!");
    // send(_client_socket, _buffer, sizeof(_buffer), 0); // 发送的数据 

    // TODO: 连续读图片
    ReceiveImage();

    Disconnet();
}


void SocketServer::ReceiveImage() {
    // 接收客户端输入
    if ((_fp = fopen(_img_file_name, "wb")) == NULL) {
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
    }
    printf("Image received.\n");
}


void SocketServer::Disconnet() {
    close(_server_socket);  
    printf("Socket disconnected.\n");
}
