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
#define MSG "hello,this is client"
int main(int c,char **v)
{
    int fd;
    struct sockaddr_un addr;
    char buf[MAXSIZE];
    int rv;

    
    fd=socket(AF_UNIX,SOCK_STREAM,0);
    
    memset(&addr,0,sizeof(addr));
    addr.sun_family=AF_UNIX;
    strncpy(&addr.sun_path[0],NAME,sizeof(addr.sun_path)-1);


    if(connect(fd,(struct sockaddr*)&addr,sizeof(addr))<0)
    {
        perror("connect");
        close(fd);
        return -1;
    }

    if((rv=read(fd,buf,sizeof(buf)))<0)
    {
        perror("read");
        close(fd);
        return -1;
    }
    else if(rv==0)
    {
        printf("connection lost\n");
        close(fd);
        return -1;
    }
    else
    {
        printf("read %d bytes data from client:%s\n",rv,buf);
    }

    strncpy(buf,MSG,sizeof(buf)-1);

    if((rv=write(fd,buf,strlen(buf)+1))<0)
    {
        perror("write");
        close(fd);
        return -1;
    }
    printf("write %d bytes data to the client:%s\n",strlen(buf)+1,buf);
    close(fd);                           
    return 0;
}
