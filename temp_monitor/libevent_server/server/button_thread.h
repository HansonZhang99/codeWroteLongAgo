/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  button_thread.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 13时17分48秒"
 *                 
 ********************************************************************************/

#ifndef __BUTTON_THREAD__
#define __BUTTON_THREAD__

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <mosquitto.h>
#include <string.h>
#include <stdlib.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "crc-itu-t.h"
#include "packet.h"
#include "mosq_ps.h"
#include "analysis.h"
#include "ctx.h"
#define EV_PRESS 1
#define PATH_1 "/dev/input/event0"

#define MESSAGE_COUNT 100000L
#define MESSAGE_SIZE 1024L

int button_thread(ctx_t *);

#endif
