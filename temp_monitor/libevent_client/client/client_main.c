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

#include <stdio.h>
#include <unistd.h>
#include <mosquitto.h>
#include <string.h>
#include <errno.h>

#include "db_init.h"
#include "ctx.h"
#include "mosq_ps.h"
#include "analysis.h"
#include "opt_init.h"
#include "insert_db.h"

#define DB_NAME "database.db"
#define FILE_NAME "temperature"


int main(int argc ,char **argv)
{
    ctx_t               ctx;
    pid_t               pid;

    if(opt_init(ctx.mqtt_username,ctx.mqtt_password,ctx.temp_topic,ctx.broker_address,&(ctx.broker_port),ctx.server_ip,&(ctx.server_port),ctx.ca_file,ctx.crt_file,ctx.key_file,argc,argv))
    {
        printf("opt_init error:[%d] [%s]\n",errno,strerror(errno));
        return -1;
    }
    pid=fork();//创建一个新的进程，一个进程用来接收broker上的mqtt消息，一个进程用来发送指令到开发板，并控制开发板的led和蜂鸣器
    if(pid<0)
    {
        printf("fork error:[%d] [%s]\n",errno,strerror(errno));
        return -1;
    }
    else if(pid==0)
    {

        login(&ctx);//创建mqtt连接，设置回调函数,当收到消息时自动进行TLV解析，并将数据保存到数据库中

        if(!ctx.mosq)
        {
            printf("login error:%s\n",strerror(errno));
            return -1;
        }

        strncpy(ctx.database_name,DB_NAME,ARR_SIZE-1);
        strncpy(ctx.db_filename,FILE_NAME,ARR_SIZE-1);

        if(db_init(ctx.database_name,ctx.db_filename))//初始化数据库
        {
            printf("db_init error:[%d] [%s]\n",errno,strerror(errno));
            return -1;
        }
        if(!(ctx.db=open_db(ctx.database_name,ctx.db_filename)))//打开数据库
        {
            printf("open_db error:[%d] [%s]\n",errno,strerror(errno));
            return -1;
        }
        if(mosq_subscribe(&ctx))//订阅topic
        {
            printf("mosquitto_subscribe error:[%d] [%s]\n",errno,strerror(errno));
        }
        sqlite3_close(ctx.db);//关闭数据库
        mosquitto_destroy(ctx.mosq);//释放资源
        mosquitto_lib_cleanup();//释放资源
        return 0;
    }
    else
    {
        if(libevent_client(&ctx))
        {
            printf("libevent_client error:[%d] [%s]\n",errno,strerror(errno));
            return -1;
        }
        return 0;
    }
}
