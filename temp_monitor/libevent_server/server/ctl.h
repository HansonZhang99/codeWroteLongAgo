/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  led_blink.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 18时27分57秒"
 *                 
 ********************************************************************************/

#ifndef __CTL_H__
#define __CTL_H__

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#define BUZZER_PATH "/dev/s3c2440_buzzer"

#ifndef ON
#define ON 1
#endif

#ifndef OFF 
#define OFF 0
#endif

void ctl(int cmd);

#endif
