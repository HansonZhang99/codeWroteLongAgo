/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  analysis.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(04/01/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "04/01/2019 05:24:49 PM"
 *                 
 ********************************************************************************/

#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include "crc-itu-t.h"
#include "ctx.h"
#include<stdio.h>
#include<string.h>
// const unsigned char Head=0xfd;
#define Head 0xfd


enum
{
    TLV_STR=1,
    TLV_CHR,
    TLV_SHT,
    TLV_INT,//includes short and char ,just need more space
    TLV_DOU,
};

int byte_to_int(unsigned char *buf,int cmd);
void byte_to_string(unsigned char *buf,int len,unsigned char *dst);
double byte_to_double(unsigned char *buf);
int analysis_TLV_msg(unsigned char *buf,int rv,char *);

#endif
