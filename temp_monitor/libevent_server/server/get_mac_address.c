/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  get_mac_address.c
 *    Description:  This file i
 *                 
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 12时10分45秒"
 *                 
 ********************************************************************************/


/*--------------------------------------------------------
 *Description:
 *This funtion is used to get the host's mac address,and 
 *you should export a array which is more than 17 bytes,
 *after excuting this function,this mac address will copy 
 *into the array which you exported
 *------------------------------------------------------*/

#include "get_mac_address.h"

int get_mac_address(char mac[])
{
    if(!mac)
    {
        printf("Invalid argument mac\n");
        return -1;
    }
    int fd;
    char *ptr="ifconfig eth0 > ";
    char buf[1024]={0};
    snprintf(buf,1024,"%s %s",ptr,MAC_FILE);
    int val=system(buf);
    if(val)
    {
        printf("system error\n");
        return -1;
    }
    if((fd=open(MAC_FILE,O_RDWR))<0)
    {
        printf("open failure\n");
        return -1;
    }
    lseek(fd,0,SEEK_SET);
    int rv=read(fd,buf,sizeof(buf));
    if(rv<0)
    {
        printf("read failure\n");
        return -1;
    }
    char *str=strstr(buf,"HWaddr");
    if(!str)
    {
        printf("no mac address\n");
        return -1;
    }
    str+=6;
    while((*str)==' ')
        str++;
    memcpy(mac,str,17);
    mac[17]='\0';
    strncpy(buf,"rm ",sizeof(buf));
    strncat(buf,MAC_FILE,sizeof(buf));
    val=system(buf);
    if(val)
    {
        printf("system error\n");
        return -1;
    }
    close(fd);
    return 0;
}
