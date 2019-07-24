/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  buzzer.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 18时17分19秒"
 *                 
 ********************************************************************************/

#include "buzzer.h"

void *buzzer(void *args)
{
    int fd=open(BUZZER_PATH,O_RDWR);
    int count=3;
    if(fd<0)
    {
        printf("open %s error\n",BUZZER_PATH);
        pthread_exit(NULL);
    }
    while(count--)
    {
        ioctl(fd,ON,0);
        sleep(1);
        ioctl(fd,OFF,0);
        sleep(1);
    }
    close(fd);
    return NULL;
}
    
