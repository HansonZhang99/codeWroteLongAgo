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
#include <stdio.h>
#include <unistd.h>
#include <mosquitto.h>
#include "ctx.h"
#include "mosq_ps.h"
#include "libevent_server.h"

int main(int argc,char **argv)
{
    ctx_t               ctx;
    struct              mosquitto *mosq;
    pid_t               pid;

    if(opt_init(ctx.mqtt_username,ctx.mqtt_password,ctx.temp_topic,ctx.broker_address,&(ctx.broker_port),ctx.server_ip,&(ctx.server_port),ctx.ca_file,ctx.crt_file,ctx.key_file,argc,argv))
    {
        printf("opt_init error:[%d] [%s]\n",errno,strerror(errno));
        return -1;
    }

    if(get_mac_address(ctx.mac_address))
    {
        return -1;
    }
    
    login(&ctx);
  
    if(!ctx.mosq)
    {
        printf("login error:%s\n",strerror(errno));
        return -1;
    }

    if(button_thread(&ctx))
    {
        printf("button_thread error\n");
        return -1;
    }

    set_signal();

    pid=fork();

    if(pid<0)
    {
        printf("fork error\n");
        return -1;
    }
    else if(pid>0)
    { 
        while(1)
        {
            struct timeval tv={30,0};
            if(mosq_publish(&ctx))
            {
                printf("mosq_publish error\n");
                return -1;
            }
            select(10,NULL,NULL,NULL,&tv);
            //sleep(30);
        }
        mosquitto_destroy(ctx.mosq);
        mosquitto_lib_cleanup();
        return 0;
    } 
    else
    {
        if(libevent_server(&ctx))
        {
            printf("libevent_server error:[%d] [%s]\n",errno,strerror(errno));
            return -1;
        }
        return 0;
    }
    return 0;
}
