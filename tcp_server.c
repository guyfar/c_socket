//
//  socket_tcp.c
//  socket
//
//  Created by gaoxiaofei on 16/4/10.
//  Copyright © 2016年 gaoxiaofei. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4545

#define LISTEN_LEN 3        //监听的队列长度
#define MAXRECVLEN 1024     //数据传送的缓冲区大小

int main(int argc, char *argv[]){
    char buf[MAXRECVLEN];   //数据传送的缓冲区
    int server_socked;      //服务器端套接字
    int client_socket;      //客户端套接字
    socklen_t addrlen;
    struct sockaddr_in server;
    struct sockaddr_in client;
    
    //设置socket参数
    int opt = SO_REUSEADDR;
    setsockopt(server_socked, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    //置server存储单元为零
    bzero(&server,sizeof(server));
    
    //初始化结构体对象
    server.sin_family = AF_INET;    //使用IPV4网络协议
    server.sin_port = htons(PORT);  //把主机地址顺序转换成网络地址顺序
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //创建服务器端的套接字--IPv4协议，TCP协议
    if((server_socked=socket(PF_INET,SOCK_STREAM,0))==-1){
        perror("socket() error . \n");
        return 1;
    }
    
    //绑定套接字到服务器的网络地址上
    if(bind(server_socked, (struct sockaddr *)&server, sizeof(server))==-1){
        perror("bind() error . \n");
        return 1;
    }
    
    //监听连接请求---LISTEN_LEN:监听的队列长度
    if(listen(server_socked, LISTEN_LEN)==-1){
        perror("listen() error . \n");
        return 1;
    }
    
    addrlen = sizeof(client);
    while (1) {
        //等待客户端发送的连接请求
        if((client_socket=accept(server_socked, (struct sockaddr *)&client, &addrlen))==-1){
            perror("accept() error . \n");
            return 1;
        }
        
        //服务器端输出连接请求的信息
        printf("You get a connection from client,ip: %s,port:%d\n",inet_ntoa(client.sin_addr),htons(client.sin_port));
        
        int iret = -1;
        while (1) {
            /*接收客户端的数据并将其发送给客户端--recv返回接收到的字节数，send返回发送的字节数*/
            iret = recv(client_socket, buf, MAXRECVLEN, 0);
            if(iret > 0){
                printf("message:%s\n",buf);
            }else{
                close(client_socket);
                break;
            }
            
            send(client_socket, buf, iret, 0);
        }
    }
    
    //关闭服务器的套接字
    close(server_socked);
    return 0;
}