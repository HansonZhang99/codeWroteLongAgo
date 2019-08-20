/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  socket_client.c
 *    Description:  client
 *                 
 *        Version:  1.0.0(03/22/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/22/2019 09:51:17 PM"
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

int main(int argc, char **argv)
{
    if(argc!=3)
    {
        printf("usage: ./a.out [port] [ipaddress]\n");
        return -1;
    }
    struct sockaddr_in              server_addr;
    int                             cli_fd;
    char                            buf[MAXSIZE];
    int                             sr,sw;
    if((cli_fd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("socket error:%d:%s\n",errno,strerror(errno));
        return -1;
    }
    printf("socket OK fd[%d]\n",cli_fd);

    bzero(&server_addr,sizeof(server_addr));

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(atoi(argv[1]));
    inet_aton(argv[2],&server_addr.sin_addr);

    if(connect(cli_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
    {
        printf("connect error:%d:%s\n",errno,strerror(errno));
        close(cli_fd);
        return -1;
    }
    printf("connect to server %s:%d OK\n",argv[2],atoi(argv[1]));

    bzero(buf,sizeof(buf));
    strcpy(buf,"Hello,this is client\n");
    if((sw=write(cli_fd,buf,sizeof(buf)))<=0)
    {
        printf("write error:%d:%s\n",errno,strerror(errno));
        close(cli_fd);
        return -1;
    }
    printf("write %d bytes data to server:%s\n",(int)strlen(buf),buf);
    bzero(buf,sizeof(buf));
    sleep(1);
    if((sr=read(cli_fd,buf,sizeof(buf)))<=0)
    {
        printf("read error:%d:%s\n",errno,strerror(errno));
        close(cli_fd);
        return -1;
    }

    printf("read %d bytes data from server:%s\n",sr,buf);
    close(cli_fd);
    return 0;
}
