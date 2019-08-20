/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  libevent_client.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2019年08月18日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月18日 00时52分27秒"
 *                 
 ********************************************************************************/

#ifndef __LIBEVENT_CLIENT__
#define __LIBEVENT_CLIENT__

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
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <event2/event.h>
#include "ctx.h"


int libevent_client(ctx_t *g_ctx);

#endif
