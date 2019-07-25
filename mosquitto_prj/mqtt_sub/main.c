/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  main.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月24日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月24日 13时22分03秒"
 *                 
 ********************************************************************************/

#include "opt_init.h"
#include "login.h"
#include "get_mac_address.h"
#include "mosq_subscribe.h"
#include <stdio.h>
#include <unistd.h>
#include <mosquitto.h>
#include "analysis.h"
#include <string.h>
#include <errno.h>
#include "db_init.h"

int main(int argc ,char **argv)
{
    char                username[ARR_SIZE]={0};
    char                temp_topic[ARR_SIZE]={0};
    char                warn_topic[ARR_SIZE]={0};
    char                password[ARR_SIZE]={0};
    char                address[ARR_SIZE]={0};
    int                 port=0;
    struct              login_t login;
    struct              mosquitto    *mosq;
    char                mac[ARR_SIZE];
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
    if(db_init(DB_NAME,FILE_NAME))
    {
        return -1;
    }
    if((login.db=open_db(DB_NAME,FILE_NAME))==NULL)
    {
        return -1;
    }
    mosq=login_mqtt(&login);
    if(!mosq)
    {
        printf("login error:%s\n",strerror(errno));
        return -1;
    }

    if(mosq_subscribe(mosq,&login))
    {
        printf("mosq_subscribe failure:%s \n",strerror(errno));
    }
    sqlite3_close(login.db);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}
