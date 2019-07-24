/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  packet.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(04/01/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "04/01/2019 05:34:04 PM"
 *                 
 ********************************************************************************/

#ifndef _PACKET_H_
#define _PACKET_H_
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#include"crc-itu-t.h"
#include"packet.h"
#include"analysis.h"
int create_TLV_msg(unsigned char buf[],int buf_len,int Tag,void *value,int num);

#endif
