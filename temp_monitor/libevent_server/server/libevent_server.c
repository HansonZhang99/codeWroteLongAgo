/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  server.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月16日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月16日 23时20分22秒"
 *                 
 ********************************************************************************/

#include "libevent_server.h"

void ev_error_cb(struct evconnlistener *listener,void *args)
{
    struct event_base* base=evconnlistener_get_base(listener);
    int err=EVUTIL_SOCKET_ERROR();
    printf("[libevent] Got an error %d (%s) on the listener.Shutting down.\n", err, evutil_socket_error_to_string(err));
    event_base_loopexit(base,NULL);//运行完所有激活回调后立刻停止循环
}

void ssl_read_cb(struct bufferevent *bev,void *ctx)
{
    char buf[MAXSIZE];
    int n;
    struct evbuffer *input=bufferevent_get_input(bev);//获取输入缓冲区
    while((n=evbuffer_remove(input,buf,MAXSIZE))>0)
    {
        printf("[libevent] read %d bytes data: %s\n",n-1,buf);
    }
    if(!strcmp(buf,"1")||!strcmp(buf,"2")||!strcmp(buf,"3")||!strcmp(buf,"4")||!strcmp(buf,"5")||!strcmp(buf,"6")||\
            !strcmp(buf,"0x01")||!strcmp(buf,"0x02")||!strcmp(buf,"0x03")||!strcmp(buf,"0x04")||!strcmp(buf,"0x05")||!strcmp(buf,"0x06"))
    {
        int val=atoi(buf);
        printf("%d\n",val);
        ctl(val);
        bufferevent_write(bev,"[libevent] server:command has been excuted\n",strlen("[libevent] server:command has been excuted\n")+1);
    }
    else
    {
        bufferevent_write(bev,"[libevent] server:this is not a vaild command\n",strlen("[libevent] server:this is not a vaild command\n")+1);
    }
    putchar('\n');
}

void ssl_event_cb(struct bufferevent* bev,short ev,void *ctx)
{
    if(ev & BEV_EVENT_CONNECTED)
    {
        printf("[libevent] connect with client with fd [%d]\n",bufferevent_getfd(bev));
    }
    else if(ev & BEV_EVENT_ERROR)
    {
        printf("[libevent] something error on server ,and libevent server program will exit now\n");
        bufferevent_free(bev);
    }
}

void ssl_acceptcb(struct evconnlistener *listener,int fd,struct sockaddr *addr,int socklen,void *ctx)
{
    SSL_CTX *server_ctx=(SSL_CTX*)ctx;//获取传入的ssl句柄
    SSL * ssl=SSL_new(server_ctx);

    char *str="[libevent] welcome to the server!\n \
now you can write some order to the server \n \
to control the buzzer and the led :\n \
order :       effect:\n \
(1)           led on\n \
(2)           led blink\n \
(3)           led heart_blink\n \
(4)           led off\n \
(5)           buzzer warning\n ";

    struct event_base *base=evconnlistener_get_base(listener);

    //struct bufferevent *bev=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    struct bufferevent *bev=bufferevent_openssl_socket_new(base,fd,ssl,BUFFEREVENT_SSL_ACCEPTING,BEV_OPT_CLOSE_ON_FREE);
    if(!bev)
    {
        printf("[libevent] bufferevent_openssl_socket_new error\n");
        event_base_loopexit(base, NULL);
    }
    bufferevent_write(bev,str,strlen(str)+1);
    bufferevent_setcb(bev,ssl_read_cb,NULL,ssl_event_cb,NULL);//这里只是读取从客户端来的数据，并不需要传入参数
    bufferevent_enable(bev,EV_READ);//bufferevent默认只开启写回调
}



static SSL_CTX * evssl_init(ctx_t* g_ctx)
{
    if(!g_ctx)
        return NULL;
    SSL_CTX *server_ctx;
    SSL_load_error_strings();
    SSL_library_init();
    if(!RAND_poll())
    {
        return NULL;
    }

    server_ctx=SSL_CTX_new(SSLv23_server_method());
    if(!server_ctx)
    {
        SSL_CTX_free(server_ctx);//释放堆区内存
        printf("[libevent] SSL_CTX_new error\n");
        return NULL;
    }

    SSL_CTX_set_verify(server_ctx, SSL_VERIFY_PEER, NULL);

    if(!SSL_CTX_load_verify_locations(server_ctx,g_ctx->ca_file,NULL)|| \
            !SSL_CTX_use_certificate_chain_file(server_ctx,g_ctx->crt_file)|| \
            !SSL_CTX_use_PrivateKey_file(server_ctx,g_ctx->key_file,SSL_FILETYPE_PEM))
    {
        SSL_CTX_free(server_ctx);//释放堆区内存
        printf("[libevent] error while loading necessary files\n");
        return NULL;
    }

//    SSL_CTX_set_default_passwd_cb_userdata(server_ctx, g_ctx->ssl_passwd);
    
    if(!SSL_CTX_check_private_key(server_ctx))
    {
        SSL_CTX_free(server_ctx);//释放堆区内存
        printf("[libevent] SSL_CTX_check_private_key error\n");
        return NULL;
    }

    return server_ctx;
}

int libevent_server(ctx_t* g_ctx)
{
    SSL_CTX *ctx;
    struct evconnlistener *listener;
    struct event_base *base;
    struct sockaddr_in addr;
    
    memset(&addr,0,sizeof(addr));

    addr.sin_family=AF_INET;
    addr.sin_port=htons(g_ctx->server_port);
    //addr.sin_addr.s_addr=inet_addr(g_ctx->server_ip);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    ctx=evssl_init(g_ctx);
    if(!ctx)
    {
        printf("[libevent] evssl_init error\n");
        return -1;
    }

    base=event_base_new();//Linux下默认使用epoll后端
    if(!base)
    {
        printf("[libevent] event_base_new error\n");
        return -1;
    }
    //flag:LEV_OPT_CLOSE_ON_FREE 释放连接关闭底层套接字
    //flag:LEV_OPT_REUSEABLE 让套接字立刻可以复用
    //当有新的连接到来时，回调函数ssl_acceptcb会被调用，并传入参数ctx
    //参数13表示网络栈允许处于还未接受状态的最大未决连接数
    listener=evconnlistener_new_bind(base,ssl_acceptcb,(void *)ctx,LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,13,(struct sockaddr*)&addr,sizeof(addr));
    if(!listener)
    {
        printf("[libevent] evconnlistener_new_bind error\n");

        return -1;
    }
    evconnlistener_set_error_cb(listener,ev_error_cb);
    event_base_loop(base,0);//循环调用多路复用，直到多路复用中再无事件
    event_base_free(base);
    evconnlistener_free(listener);//释放堆区内存
    SSL_CTX_free(ctx);//释放堆区内存
    return 0;
}
