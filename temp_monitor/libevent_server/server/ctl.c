/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  led_blink.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 18时26分48秒"
 *                 
 ********************************************************************************/

#include "ctl.h"

void ctl(int  cmd)
{
    int fd=open(BUZZER_PATH,O_RDWR);
    if(fd<0)
    {
        printf("open %s error\n",BUZZER_PATH);
        pthread_exit(NULL);
    }
    switch(cmd)
    {
        case 1:
            if(system("echo 1 > /sys/class/leds/led1/brightness")==-1 || \
                    system("echo 1 > /sys/class/leds/led2/brightness")==-1 || \
                    system("echo 1 > /sys/class/leds/led3/brightness")==-1 || \
                    system("echo 1 > /sys/class/leds/led4/brightness")==-1)
            {
                printf("system error:[%d] [%s]\n",errno,strerror(errno));
                return ;
            }
            break;
        case 2:
            if(system("echo 0 > /sys/class/leds/led1/brightness")==-1 || \
                    system("echo 0 > /sys/class/leds/led2/brightness")==-1 || \
                    system("echo 0 > /sys/class/leds/led3/brightness")==-1 || \
                    system("echo 0 > /sys/class/leds/led4/brightness")==-1)
            {
                printf("system error:[%d] [%s]\n",errno,strerror(errno));
                return ;
            }
            break;
        case 3:
            if(system("echo timer > /sys/class/leds/led1/trigger")==-1 || \
                    system("echo timer > /sys/class/leds/led2/trigger")==-1 || \
                    system("echo timer > /sys/class/leds/led3/trigger")==-1 || \
                    system("echo timer > /sys/class/leds/led4/trigger")==-1)
            {
                printf("system error:[%d] [%s]\n",errno,strerror(errno));
                return ;
            }
            break;
        case 4:
            if(system("echo heartbeat > /sys/class/leds/led1/trigger")==-1 || \
                    system("echo heartbeat > /sys/class/leds/led2/trigger")==-1 || \
                    system("echo heartbeat > /sys/class/leds/led3/trigger")==-1 || \
                    system("echo heartbeat > /sys/class/leds/led4/trigger")==-1)
            {
                printf("system error:[%d] [%s]\n",errno,strerror(errno));
                return ;
            }
            break;
        case 5:
            ioctl(fd,ON,0);
            break;
        case 6:
            ioctl(fd,OFF,0);
            break;
        default:
            printf("Invaild command\n");
            break;
    }
    close(fd);
}
