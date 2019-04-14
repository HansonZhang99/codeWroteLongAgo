/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  test_main.c
 *    Description:  This file 
 *            
 *        Version:  1.0.0(03/30/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/30/2019 01:53:30 PM"
 *                 
 ********************************************************************************/
#include<stdio.h>
#include<string.h>
#include"packet.h"
#include"analysis.h"

//#define MAXSIZE 1024
int main()
{
    unsigned char buf[MAXSIZE];
    memset(buf,0,sizeof(buf));
    int         a=128;
    int         b=-24324;
    char        dst[16];
    int         c=0;
    char        f=173;
    char        v=23;
    short       g=-13432;
    int         len;
    double      d=-113.4333;
    double      h=0.9;
    double      e=(23*0.9);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Hello",0);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Hi",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"This flower smells good,do you sell them?",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"How much is it?",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_CHR,&v,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Can you give me a discount?",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_DOU,&h,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Ok,I wanna to 8",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Ok, Here you are",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_DOU,&e,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"This is the money,thanks!",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"+++++++++++++++++++++++++++++++++++++++",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"test begins:",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"a int:",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_INT,&a,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"a double:",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_DOU,&d,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"a big string:",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"You just want my attention,you don't want my heart,you just making sure I never getting over    you",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"a double:",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_DOU,&h,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"an another big string",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Hello,if you want to leave,I have no idea to stop you to do this.",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"a short:",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_SHT,&g,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"a string:",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Hello",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"a unsigned char:",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_CHR,&f,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"test ends",len);
    int i;
    for(i=0;i<len;i++)
    {
        if(i%32==0)
            putchar('\n');
        printf("0x%02X,",buf[i]);
    }
    analysis_TLV_msg(buf,len);

    return 0;
}
