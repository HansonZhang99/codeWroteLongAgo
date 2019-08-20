/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  mosq_publish.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 15时29分42秒"
 *                 
 ********************************************************************************/

#ifndef __MOSQ_PS_H_
#define __MOSQ_PS_H_

#include <mosquitto.h>
#include "get_mac_address.h"
#include "get_temperature.h"
#include "crc-itu-t.h"
#include "packet.h"
#include "create_json_data.h"
#include "analysis.h"
#include "button_thread.h"
#include <string.h>
#include <errno.h>
#include "ctx.h"


int mosq_publish(ctx_t *ctx);
int mosq_subscribe(ctx_t *ctx);
int login(ctx_t *ctx);
#endif
