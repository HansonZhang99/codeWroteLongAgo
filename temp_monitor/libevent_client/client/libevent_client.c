/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月17日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月17日 13时29分45秒"
 *                 
 ********************************************************************************/

#include "libevent_client.h"

static SSL_CTX * evssl_init(ctx_t* g_ctx)
{
    if(!g_ctx)
        return NULL;
    SSL_CTX *client_ctx;
    SSL_load_error_strings();
    SSL_library_init();
    if(!RAND_poll())
    {
        return NULL;
    }

    client_ctx=SSL_CTX_new(SSLv23_client_method());
    if(!client_ctx)
    {
        SSL_CTX_free(client_ctx);//释放堆区内存
        printf("[libevent] SSL_CTX_new error\n");
        return NULL;
    }

    SSL_CTX_set_verify(client_ctx, SSL_VERIFY_PEER, NULL);

    if(!SSL_CTX_load_verify_locations(client_ctx,g_ctx->ca_file,NULL)|| \
            !SSL_CTX_use_certificate_chain_file(client_ctx,g_ctx->crt_file)|| \
            !SSL_CTX_use_PrivateKey_file(client_ctx,g_ctx->key_file,SSL_FILETYPE_PEM))
    {
        SSL_CTX_free(client_ctx);//释放堆区内存
        printf("[libevent] error while loading necessary files\n");
        return NULL;
    }


    if(!SSL_CTX_check_private_key(client_ctx))
    {
        SSL_CTX_free(client_ctx);//释放堆区内存
        printf("[libevent] SSL_CTX_check_private_key error\n");
        return NULL;
    }
    return client_ctx;
}


void ssl_read_cb(struct bufferevent *bev,void *args)
{   
    char buf[1024];
    int n;
    struct evbuffer *input=bufferevent_get_input(bev);//获取输入缓冲区
    while((n=evbuffer_remove(input,buf,MAXSIZE))>0)
    {
        fwrite(buf,n,1,stdout);
    }

    fgets(buf,sizeof(buf)-1,stdin);//从标准输入读取数据到buf,考虑到这是客户端的原因，并不需要类似服务器的高效率，程序可以阻塞
    char *ch=strchr(buf,'\n');
    if(ch)
    {
        *ch='\0';
    }//消除fgets函数的多余换行符
    bufferevent_write(bev,buf,strlen(buf)+1);//发送指令到server
    printf("[libevent] write command :[%s] to the server\n",buf);
    fflush(stdin);//刷新输入缓冲区
}

void ssl_event_cb(struct bufferevent *bev,short ev,void *ptr)
{
    if(ev & BEV_EVENT_CONNECTED)
    {
        printf("[libevent] build openssl connection with the server successfully\n");
    }
    else if(ev & BEV_EVENT_ERROR)
    {
        int err=EVUTIL_SOCKET_ERROR();
        printf("[libevent] something gets error,libevent client program exit:%s\n",evutil_socket_error_to_string(err));
        bufferevent_free(bev);
    }
}


int libevent_client(ctx_t *g_ctx)
{
    if(!g_ctx)
    {
        return -1;
    }

    struct bufferevent* bev;
    struct event_base *base;
    int fd;
    struct sockaddr_in addr;

    memset(&addr,0,sizeof(addr));
    
    addr.sin_family=AF_INET;
    addr.sin_port=htons(g_ctx->server_port);
    addr.sin_addr.s_addr=inet_addr(g_ctx->server_ip);

    SSL_CTX * client_ctx;
    SSL * ssl;

    if((client_ctx=evssl_init(g_ctx))==NULL)
    {
        return -1;
    }
    ssl=SSL_new(client_ctx);

    fd=socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);//设置fd为非阻塞
    if(fd<0)
    {
        printf("[libevent] socket error:[%d] [%s]\n",errno,strerror(errno));
        return -1;
    }

    int one=1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));//ip地址复用

    base=event_base_new();//Linux下默认使用epoll后端
    if(!base)
    {
        printf("[libevent] event_base_new error:[%d] [%s]\n",errno,strerror(errno));
        close(fd);
        return -1;
    }
    while(connect(fd,(struct sockaddr*)&addr,sizeof(addr)));//非阻塞等待连接

    printf("[libevent] connect to server ip[%s] port[%d] successfully\n",g_ctx->server_ip,g_ctx->server_port);

    //bev=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    bev=bufferevent_openssl_socket_new(base,fd,ssl,BUFFEREVENT_SSL_CONNECTING,BEV_OPT_CLOSE_ON_FREE);//与服务器建立openssl连接
    if(!bev)
    {
        printf("[libevent] bufferevent_opennssl_socket_new error:[%d] [%s]\n",errno,strerror(errno));
        close(fd);
        event_base_free(base);
        return -1;
    }
    bufferevent_setcb(bev,ssl_read_cb,NULL,ssl_event_cb,NULL);//默认只开启写回调
    bufferevent_enable(bev,EV_WRITE|EV_READ);//使能读回调
    SSL_CTX_free(client_ctx);//释放堆区内存
    event_base_dispatch(base);
    return 0;
}

