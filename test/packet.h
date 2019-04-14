/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  packet.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(03/30/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/30/2019 02:48:56 PM"
 *                 
 ********************************************************************************/
#include"packet.h"

#ifndef _PACKET_H_
#define _PACKET_H_

extern const unsigned char Head;

extern int create_TLV_msg(char *buf,int Tag,void *value,int num);

#endif
