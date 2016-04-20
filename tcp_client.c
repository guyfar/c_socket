//
//  tcp_client.c
//  socket
//
//  Created by gaoxiaofei on 16/4/10.
//  Copyright © 2016年 gaoxiaofei. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  //struct hostent 需要用到

#define PORT 4545   //server端定义的端口

#define MAXDATASIZE 100

#define errno WSAGetLastError()

int main(int argc, char *argv[]){
    int client_sockfd;          //服务器端套接字
    int num;                    // files descriptors
    char buf[MAXDATASIZE];      //数据传送的缓冲区
    struct hostent *he;         // structure that will get information about remote host
    struct sockaddr_in server;
    
    
    //判断请求参数是否有IP Address
    if (argc != 2){
        printf("Usage: %s <IP Address>\n",argv[0]);
        exit(1);
    }
    
    if((he=gethostbyname(argv[1]))==NULL){
        printf("gethostbyname() error\n");
        exit(1);
    }
    
    //创建客户端的套接字--IPv4协议，TCP协议
    if((client_sockfd=socket(AF_INET,SOCK_STREAM, 0))==-1){
        printf("socket() error\n");
        exit(1);
    }
    
    //发送接收信息并打印接收信息--recv返回接收到的字节数，send返回发送的字节数
    printf("Enter string to send:");
    char str[90];
    gets(str);
    
    //初始化结构体对象
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr = *((struct in_addr *)he->h_addr);
    
    //将套接字绑定到服务器的网络地址上
    if(connect(client_sockfd, (struct sockaddr *)&server, sizeof(server))==-1){
        printf("connect() error:%d\n");
        exit(1);
    }
    
    if((num=send(client_sockfd,str,sizeof(str),0))==-1){
        printf("send() error\n");
        exit(1);
    }
    if((num=recv(client_sockfd,buf,MAXDATASIZE,0))==-1)
    {
        printf("recv() error\n");
        exit(1);
    }
    buf[num-1]='\0';
    printf("server message: %s\n",buf);
    close(client_sockfd);
    return 0;
}