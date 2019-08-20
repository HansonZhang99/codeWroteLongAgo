/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  test.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月29日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月29日 16时38分13秒"
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
#include <netinet/in.h>
#include "event.h"
#define MAXSIZE 1024
#define PORT 8888

struct event_rw
{
    struct event* read_event;
    struct event* write_event;
};

void free_event(struct event_rw* rv)
{
    event_free(rv->read_event);
    event_free(rv->write_event);
}

void do_read(evutil_socket_t fd,short events,void *arg)
{
    char buf[MAXSIZE];
    int res;
    struct event_rw* event_s=(struct event_rw*)arg;
    //while(1)
    {
        res=recv(fd,buf,sizeof(buf),0);
        if(res<=0)
        {
            free_event(event_s);
            return;
        }
        else
        {
            printf("read %d data from client with fd %d : %s\n",res,fd,buf);
        }
    }
 //   sleep(1);
}

void do_write(evutil_socket_t fd,short events,void *arg)
{
    char buf[MAXSIZE]="hello";
    int res;
    struct event_rw* event_s=(struct event_rw*)arg;
    res=send(fd,buf,sizeof(buf),0);
    if(res>0)
    {
        printf("write %d data to client with fd %d : %s\n",res,fd,buf);
    }
    else if(res<=0)
    {
        free_event(event_s);
        return ;
    }
   // sleep(1);
}


void do_signal(evutil_socket_t fd,short events,void *arg)
{
    printf("get interrupt signal,the program is exiting\n");
    exit(0);
}

void do_accept(evutil_socket_t listen_fd,short event,void *arg)
{
    struct event_base *base=(struct event_base*)arg;
    struct sockaddr_storage addr;
    socklen_t addr_len=sizeof(addr);
    int accept_fd=accept(listen_fd,(struct sockaddr*)&addr,&addr_len);
    if(accept_fd<0)
    {
        printf("accept error\n");
    }
    else if(accept_fd>FD_SETSIZE)
    {
        close(accept_fd);
    }
    else
    {
        printf("lis_fd: %d acc_fd %d\n",listen_fd,accept_fd);
        evutil_make_socket_nonblocking(accept_fd);
        struct event_rw * event=(struct event_rw * )malloc(sizeof(struct event_rw));
        event->read_event=event_new(base,accept_fd,EV_READ|EV_PERSIST,do_read,(void *)event);
        event->write_event=event_new(base,accept_fd,EV_WRITE|EV_PERSIST,do_write,(void*)event);
        event_add(event->read_event,NULL);
        event_add(event->write_event,NULL);
    }
    return ;
}
 
int create_socket(void)
{
    evutil_socket_t  listen_fd;
    struct sockaddr_in addr;
    struct event_base *base;
    struct event *listen_event;

    listen_fd=socket(AF_INET,SOCK_STREAM,0);
    evutil_make_socket_nonblocking(listen_fd);
    if(listen_fd<0)
    {
        fprintf(stderr,"socket error\n");
        return -1;
    }
    int one = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(listen_fd,(struct sockaddr *)&addr,sizeof(addr))<0)
    {
        fprintf(stderr,"bind error:%s\n",strerror(errno));
        goto clean;
    }
    if(listen(listen_fd,16)<0)
    {
        fprintf(stderr,"listen error\n");
        goto clean;
    }

    base=event_base_new();
    if(!base)
    {
        fprintf(stderr,"event_base_new error\n");
        goto clean;
    }
    listen_event=event_new(base,listen_fd,EV_READ|EV_PERSIST,do_accept,(void *)base);
    if(!listen_event)
    {
        fprintf(stderr,"event_new error\n");
        goto clean;
    }
    event_add(listen_event,NULL);//NULL事件永不超时
    event_base_dispatch(base);
    return 0;
    
clean:
    close(listen_fd);
    return -1;
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    create_socket();
    return 0;
}
