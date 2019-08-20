/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  named_socket_server.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月11日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月11日 13时56分48秒"
 *                 
 ********************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MAXSIZE 1024
#define NAME "IPC"
#define MSG "hello,this is server"
int main(int c,char **v)
{
    int listen_fd,conn_fd;
    struct sockaddr_un addr;
    struct sockaddr_un cli_addr;
    socklen_t   len=1;
    char buf[MAXSIZE];
    int rv;

    
    listen_fd=socket(AF_UNIX,SOCK_STREAM,0);
    if(listen_fd<0)
    {
        perror("socket");
        return -1;
    }
    memset(&addr,0,sizeof(addr));
    addr.sun_family=AF_UNIX;
    strncpy(&addr.sun_path[0],NAME,sizeof(addr.sun_path)-1);

    if(bind(listen_fd,(struct sockaddr*)&addr,sizeof(addr))<0)
    {
        perror("bind");
        close(listen_fd);
        return -1;
    }

    if(listen(listen_fd,13)<0)
    {
        perror("listen");
        close(listen_fd);
        return -1;
    }

    if((conn_fd=accept(listen_fd,(struct sockaddr*)&cli_addr,&len))<0)
    {
        perror("accept");
        close(listen_fd);
        return -1;
    }

    strncpy(buf,MSG,sizeof(buf)-1);

    if((rv=write(conn_fd,buf,strlen(buf)+1))<0)
    {
        perror("write");
        close(listen_fd);
        close(conn_fd);
        return -1;
    }
    printf("write %d bytes data to the client:%s\n",strlen(buf)+1,buf);
    
    if((rv=read(conn_fd,buf,sizeof(buf)))<0)
    {
        perror("read");
        close(listen_fd);
        close(conn_fd);
        return -1;
    }
    else if(rv==0)
    {
        printf("connection lost\n");
    }
    else
    {
        printf("read %d bytes data from client:%s\n",rv,buf);
    }

    close(listen_fd);                           
    close(conn_fd);   
    return 0;
}
