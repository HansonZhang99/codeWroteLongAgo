/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  normal_client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月31日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月31日 13时17分47秒"
 *                 
 ********************************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<event2/event.h>
#include <string.h>

#define PORT 8888
#define IP_ADDRESS "127.0.0.1"
#define MAX_SIZE 1024


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
    struct event_rw* event=arg;
    int rv;
    char buf[MAX_SIZE];
    //while(1)
    {
        rv=recv(fd,buf,sizeof(buf),0);
        if(rv<=0)
        {
            free_event(event);
            return ;
        }
        printf("recieve %d bytes data from server :%s\n",rv,buf);
    }
    sleep(1);
}


void do_write(evutil_socket_t fd,short events,void *arg)
{
    struct event_rw* event=arg;
    char *str="hello,this is client\n";
    int rv;
    rv=send(fd,str,strlen(str)+1,0);
    if(rv<0)
    {
        printf("write error\n");
        free_event(event);
        return ;
    }
    printf("send %d bytes data to the server:%s\n",(int)(strlen(str)+1),str);
    sleep(1);
}

int main(int c,char **v)
{
    int fd;
    struct sockaddr_in addr;
    struct event_base* base;
    struct event* read_event;
    struct event* write_event;
    fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd<0)
    {
        printf("socket error\n");
        return -1;
    }
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    //addr.sin_addr.s_addr=inet_addr(IP_ADDRESS);
    inet_aton(IP_ADDRESS,&addr.sin_addr);
    if(connect(fd,(struct sockaddr*)&addr,sizeof(addr))<0)
    {
        printf("connect error\n");
        close(fd);
        return -1;
    }
    printf("connect t server ip[%s] port[%d] successfully\n",IP_ADDRESS,PORT);
    base=event_base_new();
    if(!base)
    {
        printf("event_base_new error\n");
        close(fd);
        return -1;
    }
    //evutil_make_socket_nonblocking(fd);
    struct event_rw * event=(struct event_rw * )malloc(sizeof(struct event_rw));
    // read_event=event_new(base,fd,EV_READ|EV_PERSIST,do_read,event_self_cbarg());
    // write_event=event_new(base,fd,EV_READ|EV_PERSIST,do_write,event_self_cbarg());
    event->read_event=event_new(base,fd,EV_READ|EV_PERSIST,do_read,(void *)event);
    event->write_event=event_new(base,fd,EV_WRITE|EV_PERSIST,do_write,(void*)event);
    if(! event->read_event||!event->write_event)
    {
        return -1;
    }
    event_add(event->read_event,NULL);
    event_add(event->write_event,NULL);
    //event_priority_set(read_event,1);
    //event_priority_set(write_event,0);
    //event_add(read_event,NULL);
    //event_add(write_event,NULL);
    event_base_dispatch(base);
    return 0;
}
