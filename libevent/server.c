/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月07日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月07日 16时02分09秒"
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

void do_read(int ,short,void*);
void do_write(int ,short,void*);

struct event_t
{
    struct event *read_event;
    struct event *write_event;
};


void do_accept(int fd,short ev,void *args)
{
    struct event_base *base=event_get_base((struct event*)args);
    struct sockaddr_in addr;
    socklen_t addr_len=sizeof(addr);
    memset(&addr,0,sizeof(addr));
    int accept_fd;
    accept_fd=accept(fd,(struct sockaddr*)&addr,&addr_len);
    if(accept_fd<0)
    {
        perror("accept error");

    }
    else if(accept_fd>FD_SETSIZE)
    {
        close(accept_fd);
    }
    else
    {
        //struct event* read_event;
        //struct event* write_event;
        struct event_t *ev=(struct event_t*)malloc(sizeof(struct event_t));
        printf("accept new client with fd %d ,and listen fd is %d\n",accept_fd,fd);
        evutil_make_socket_nonblocking(accept_fd);

        ev->read_event=event_new(base,accept_fd,EV_READ|EV_PERSIST,do_read,(void *)ev);
        ev->write_event=event_new(base,accept_fd,EV_WRITE|EV_PERSIST,do_write,(void *)ev);
        event_add(ev->read_event,NULL);
        event_add(ev->write_event,NULL);
    }
    return ;
}

void do_read(int fd,short ev,void *arg)
{
    char buf[MAXSIZE];
    struct event_t* events=(struct event_t *)arg;
    int res=recv(fd,buf,sizeof(buf),0);
    if(res<=0)
    {
        event_free(events->read_event);
        event_free(events->write_event);
        return ;
    }
    else
    {
        sleep(1);
        printf("read %d bytes data from client :",res);
        fwrite(buf,1,res,stdout);
        putchar('\n');
        event_add(events->write_event,NULL);
    }
}


void do_write(int fd,short ev,void *arg)
{
    char *str="hello,this is server";
    struct event_t* events=(struct event_t*)arg;
    int res=send(fd,str,strlen(str)+1,0);
    if(res<=0)
    {
        perror("send data to client error");
        event_free(events->read_event);
        event_free(events->write_event);
        return ;
    }
    else
    {
        printf("send %d bytes data to client :",(int)(strlen(str+1)));
        fwrite(str,1,strlen(str)+1,stdout);
        putchar('\n');
        event_del(events->write_event);
    }
}


void do_readfile(int fd,short ev,void *arg)
{
    struct event *events=(struct event*)arg;
    char buf[MAXSIZE]={0};
    int res=read(fd,buf,sizeof(buf)-1);
    if(res<=0)
    {
        event_free(events);
    }
    else
    {
        int file_fd=open("new.txt",O_CREAT|O_RDWR|O_APPEND,0666);
        if(file_fd<0)
        {
            perror("open error");
            return ;
        }
        buf[MAXSIZE-1]='\0';
        write(file_fd,buf,res);
        close(file_fd);
    }
    event_free(events);
}

void signal_fun(int sig,short ev,void *arg)
{
    struct event_base *base=(struct event_base*)arg;
    struct timeval three_sec={3,0};
    printf("catch a interrupt signal,program is exiting after three sec\n");
    event_base_loopexit(base,&three_sec);
}

void active_fun(int sock,short ev,void *arg)
{
    struct event* events=(struct event*)arg;

    printf("this event will be excuted by function once_event\n");
    event_free(events);
}

void once_event(int sock,short ev,void* arg)
{
    struct event *events=(struct event*)arg;
    printf("this fun will be excuted after five_sec\n");
    event_active(events,EV_TIMEOUT,0);
}

int main(int c,char **v)
{
    struct sockaddr_in addr;
    int listen_fd;
    int file_fd;
    struct event_base *base;
    struct event_config* cfg;
    struct event *read_event;
    struct event *listen_event;
    struct event *signal_event;
    struct event *active_event;

    file_fd =open("test.txt",O_RDONLY);
    if(file_fd<0)
    {
        perror("open file test.txt error");
        return -1;
    
    }
    listen_fd=socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd<0)
    {
        perror("socket error");
        return -1;
    
    }
    int one = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    memset(&addr,0,sizeof(addr));

    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);

    evutil_make_socket_nonblocking(listen_fd);
    evutil_make_socket_nonblocking(file_fd);

    if(bind(listen_fd,(struct sockaddr*)&addr,sizeof(addr))<0)
    {
        perror("bind error");
        close(listen_fd);
        close(file_fd);
        return -1;
    }
    
    if(listen(listen_fd,16)<0)
    {
        perror("listen error");
        close(listen_fd);
        close(file_fd);
        return -1;
    }
    
    cfg=event_config_new();
    if(!cfg)
    {
        perror("event_config_new error\n");
        close(listen_fd);
        close(file_fd);
    }

    event_config_avoid_method(cfg,"select");//avoid to use select 
	event_config_require_features(cfg,EV_FEATURE_FDS);
    event_config_set_flag(cfg,EVENT_BASE_FLAG_NOLOCK);//不分配线程锁

    base=event_base_new_with_config(cfg);
    
    event_config_free(cfg);//free cfg
    if(!base)
    {
        perror("event_base_new_with_config error\n");
        close(listen_fd);
        close(file_fd);
        return -1;
    }

    listen_event=(struct event*)malloc(sizeof(struct event));//use event_assign need alloc space self
    
    read_event=event_new(base,file_fd,EV_READ|EV_PERSIST,do_readfile,event_self_cbarg());//create a new event for copying a file
    if(!read_event)
    {
        perror("event_new error");
        goto clean;
    }
  
    event_assign(listen_event,base,listen_fd,EV_READ|EV_PERSIST,do_accept,event_self_cbarg());//create a new event for accept new client
    if(!listen_event)
    { 
        perror("event_new error");
        free(listen_event);
        event_free(read_event);
        goto clean;
    }
    
    signal_event=event_new(base,SIGINT,EV_SIGNAL|EV_PERSIST,signal_fun,(void*)base);//create a new event for interrupt signal
    if(!signal_event)
    {
        event_free(read_event);
        event_free(listen_event);
        perror("event_new error");
        goto clean;
    }
    
    active_event=event_new(base,-1,EV_PERSIST,active_fun,event_self_cbarg());//create a event which is need to be activated
    if(!active_event)
    {
        event_free(read_event);
        event_free(listen_event);
        event_free(active_event);
        perror("event_new error");
        goto clean;
    }
    
    struct timeval three_sec={3,0};
    event_base_once(base,-1,EV_READ,once_event,(void *)active_event,&three_sec);//指定超时时间过后再运行一次此event，运行完成后即释放
    
    event_add(listen_event,NULL);
    event_add(active_event,NULL);
    event_add(read_event,NULL);
    event_add(signal_event,NULL);
    
    event_base_dispatch(base);//事件循环，直到其中再无事件存在退出
    //event_base_loop(base, EVLOOP_NO_EXIT_ON_EMPTY);


clean:
    event_base_free(base);
    close(listen_fd);
    close(file_fd);
    return 0;
}
