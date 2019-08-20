/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  libevent_server.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2019年08月17日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月17日 21时11分56秒"
 *                 
 ********************************************************************************/
#ifndef __LIBEVENT_SERVER__
#define __LIBEVENT_SERVER__

#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <event2/listener.h>
#include <event2/bufferevent_ssl.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include "ctx.h"
#include "ctl.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <event2/event.h>
#define MAXSIZE 1024

int  libevent_server(ctx_t* g_ctx);

#endif


