/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  main.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 14时09分02秒"
 *                 
 ********************************************************************************/


#include "opt_init.h"
#include "set_signal.h"
#include "login.h"
#include <stdio.h>
#include <unistd.h>
#include <mosquitto.h>
#include "analysis.h"
#include "mosq_subscribe.h"


int main(int argc,char **argv)
{
    char                username[ARR_SIZE]={0};
    char                temp_topic[ARR_SIZE]={0};
    char                warn_topic[ARR_SIZE]={0};
    char                password[ARR_SIZE]={0};
    char                address[ARR_SIZE]={0};
    int                 port=0;
    struct              login_t login;
    struct              mosquitto    *mosq;
    int                 loop=0;
    pid_t               pid;
    char                mac[ARR_SIZE];
    char                temp[ARR_SIZE];
    if(opt_init(&port,username,password,temp_topic,warn_topic,address,argc,argv))
    {
        return -1;
    }


    strncpy(login.username,username,ARR_SIZE);
    strncpy(login.password,password,ARR_SIZE);
    strncpy(login.address,address,ARR_SIZE);
    strncpy(login.temp_topic,temp_topic,ARR_SIZE);
    strncpy(login.warn_topic,warn_topic,ARR_SIZE);

    login.port=port;
    if(get_mac_address(mac))
    {
        return -1;
    }
    strncpy(login.mac,mac,ARR_SIZE);
    
    mosq=login_mqtt(&login);
    if(!mosq)
    {
        printf("login error:%s\n",strerror(errno));
        return -1;
    }

    if(button_thread(mosq,&login))
    {
        printf("button_thread error\n");
        return -1;
    }

      pid=fork();

    if(pid<0)
    {
        printf("fork error\n");
        return -1;
    }
    else if(pid>0)
    {
        set_signal();
        while(1)
        {
            if(mosq_publish(mosq,&login))
            {
                printf("mosq_publish error\n");
                return -1;
            }
            sleep(10);
        }
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 0;
    } 
    else
    {
        set_signal();
        mosq=login_mqtt(&login);
        if(!mosq)
        {
            printf("login error:%s\n",strerror(errno));
            return -1;
        }

        if(mosq_subscribe(mosq,&login))
        {
            printf("mosq_subscribe error\n");
            return -1;
        }
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
    }

    return 0;
}
