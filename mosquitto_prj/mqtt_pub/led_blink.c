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

#include "led_blink.h"

void *led_blink(void *args)
{
    int a=system("echo heartbeat > /sys/class/leds/led1/trigger");
    int b=system("echo heartbeat > /sys/class/leds/led2/trigger");
    int c=system("echo heartbeat > /sys/class/leds/led3/trigger");
    int d=system("echo heartbeat > /sys/class/leds/led4/trigger");
    if(a||b||c||d)
    {
        printf("system error\n");
        pthread_exit(NULL);
    }
    sleep(3);
    int e=system("echo 0 > /sys/class/leds/led1/brightness");
    int f=system("echo 0 > /sys/class/leds/led2/brightness");
    int g=system("echo 0 > /sys/class/leds/led3/brightness");
    int h=system("echo 0 > /sys/class/leds/led4/brightness");
    if(e||f||g||h)
    {
        printf("system error\n");
        pthread_exit(NULL);                   
    }           
    return NULL;
}
