/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月08日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月08日 13时40分05秒"
 *                 
 ********************************************************************************/
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "event.h"

#define PORT 8888
#define IPADDRESS "127.0.0.1"
#define MAXSIZE 1024


void do_read(struct bufferevent* bev,void *ptr)
{
    char buf[MAXSIZE]={0};
    int n;
    struct evbuffer* input=bufferevent_get_input(bev);
    struct evbuffer* output=bufferevent_get_output(bev);
    printf("data coming:\n");
    while((n=evbuffer_remove(input,buf,sizeof(buf)))>0)
    {
        evbuffer_add(output,buf,n);
        fwrite(buf,1,n,stdout);
    }
    putchar('\n');
}


void do_write(struct bufferevent *bev,void *ptr)
{
    printf("do_write\n");
}


void do_event(struct bufferevent *bev,short ev,void *ptr)
{
    struct event_base *base=ptr;
    if(ev & BEV_EVENT_CONNECTED)
    {
        printf("connect Ok\n");
    }
    else if(ev & BEV_EVENT_ERROR)
    {
        printf("excute do_read or do_write error\n");
        bufferevent_free(bev);
    }
}

int main(int c,char **v)
{
    struct event_base * base;
    struct sockaddr_in addr;
    struct bufferevent *bev;
    int fd;

    base=event_base_new();
    if(!base)
    {
        perror("event_base_new error");
        return -1;
    }

    fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd<0)
    {
        perror("socket error");
        return -1;
    }
    int one=1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    evutil_make_socket_nonblocking(fd);
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=inet_addr(IPADDRESS);

    bev=bufferevent_socket_new(base,fd, BEV_OPT_CLOSE_ON_FREE);
    if(!bev)
    {
        perror("bufferevent_socket_new error");
        close(fd);
        event_base_free(base);
        return -1;
    }
    bufferevent_setcb(bev,do_read,do_write,do_event,(void *)base);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    if(bufferevent_socket_connect(bev,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))<0)
    {
        perror("bufferevent_socket_connect error");
        close(fd);
        event_base_free(base);
        bufferevent_free(bev);
        return -1;
    }
    event_base_dispatch(base);

    event_base_free(base);
    printf("program exit\n");
    return 0;
}
