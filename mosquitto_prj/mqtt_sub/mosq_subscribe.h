/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  mosq_subscribe.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 18时57分28秒"
 *                 
 ********************************************************************************/

#ifndef __MOSQ_SUBSCRIBE__
#define __MOSQ_SUBSCRIBE__

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <mosquitto.h>
#include <errno.h>
#include "analysis.h"
#include "login.h"
int mosq_subscribe(struct mosquitto *mosq,struct login_t *login);

#endif
