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
        exit(1);  
    }  
    // 监听客户端连接请求，监听队列长度位5
    if(listen(_server_socket, 5) < 0) {  
        perror("Listen Error:%s\n");  
        close(_server_socket);  
        exit(1);  
    }  
    printf("Start listening...\n");
    if ((_client_socket = accept(_server_socket, (struct sockaddr*) &_client_addr, &addr_len)) < 0) {
        perror("Aceept error.\n");
        exit(1);
    }
    printf("accept client %s\n", inet_ntoa(_client_addr.sin_addr)); 
    strcpy(_buffer, "Hi, I am server!");
    send(_client_socket, _buffer, sizeof(_buffer), 0); // 发送的数据 

    // 接收客户端输入
    int len;
    while(1) { 
        len = recv(_client_socket, _buffer, (size_t)MAX_BUFFER_SIZE, 0); // 接受数据
        if (len > 0) {
            printf("%s\n", _buffer);
            if (strcmp(_buffer, "Close") == 0) { // 关闭连接
                Disconnet();
                break;
            }
            strcat(_buffer, " is received.\n");  
            send(_client_socket, _buffer, sizeof(_buffer), 0); // 发送的数据
        }
         
    }  
    close(_client_socket);
}


void SocketServer::Disconnet() {
    close(_server_socket);  
    printf("Socket disconnected.\n");
}
