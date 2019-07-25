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

#ifndef __LED_BLINK__
#define __LED_BLINK__

#include <stdlib.h>
#include <pthread.h>
#include "unistd.h"
#include <stdio.h>
void * led_blink(void*);

#endif
