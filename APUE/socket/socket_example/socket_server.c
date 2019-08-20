/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  socket_server_s.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/22/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/22/2019 10:07:44 PM"
 *                 
 ********************************************************************************/
#include<stdio.h>
#include<unistd.h>//read,write
#include<sys/types.h>         //socket,bind,accept
#include<sys/socket.h>//on
#include<string.h>
#include<errno.h>
#include<netinet/in.h>//struct sockaddr_in
#include<arpa/inet.h>//on

#define MAXSIZE  1024
int main(int argc,char *argv[])
{
        int                         sock_fd,serv_fd;
        struct sockaddr_in          server_addr,client_addr;
        char                        buf[MAXSIZE];
        int                         sr,sw;
        socklen_t                   len;
        int                         optval=1;
        if(!argv[1])
        {
            puts("Usage:   ./a.out [port]\n");
            return 0;
        }

        if((sock_fd=socket(AF_INET,SOCK_STREAM,0))<0)
        {
            printf("socket error:%d:%s\n",errno,strerror(errno));
            return -1;
        }
        if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))<0)
        {
            printf("error\n");
            return -1;
        }
        printf("socket OK fd[%d]\n",sock_fd);
        bzero(&server_addr,sizeof(server_addr));
        server_addr.sin_family=AF_INET;
        server_addr.sin_port=htons(atoi(argv[1]));
        server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        //inet_aton(argv[2],&server_addr.sin_addr);
        if(bind(sock_fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))<0)
        {
            printf("bind error:%d:%s\n",errno,strerror(errno));
            close(sock_fd);
            return -1;
        }
        printf("bind OK\n");
        if(listen(sock_fd,10)<0)
        {
            printf("listen error:%d:%s\n",errno,strerror(errno));
            close(sock_fd);
            return -1;
        }
        printf("listen OK\n");

        if((serv_fd=accept(sock_fd,(struct sockaddr *)&client_addr,&len))<0)
        {
            printf("accept error:%d:%s\n",errno,strerror(errno));
            close(sock_fd);
            return -1;
        }
        printf("accept OK ip:%s   port:%d\n",inet_ntoa(client_addr.sin_addr),htons(client_addr.sin_port));

        if((sr=read(serv_fd,buf,sizeof(buf)))<=0)
        {
            printf("read error:%d:%s\n",errno,strerror(errno));
            close(serv_fd);
            close(sock_fd);
            return -1;
        }
        sleep(1);
        printf("read %d bytes data from client:%s\n",sr,buf);
        bzero(buf,sizeof(buf));
        strcpy(buf,"Hello,this is server\n");

        if((sw=write(serv_fd,buf,sizeof(buf)))<=0)
        {
            printf("write error:%d:%s\n",errno,strerror(errno));
            close(serv_fd);
            close(sock_fd);
            return -1;
        }
        printf("write %d bytes data to client:%s\n",(int)strlen(buf),buf);
        close(serv_fd);
        close(sock_fd);
        return 0;
}
        
