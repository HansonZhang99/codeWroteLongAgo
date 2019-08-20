/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  login.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2019年07月24日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月24日 13时19分23秒"
 *                 
 ********************************************************************************/

#ifndef __CTX__
#define __CTX__

#include <sqlite3.h>
#define ARR_SIZE 128
#define MAXSIZE 1024
typedef struct ctx_s
{
    char                mqtt_username[ARR_SIZE];
    char                mqtt_password[ARR_SIZE];
    char                temp_topic[ARR_SIZE];
    char                broker_address[ARR_SIZE];
    char                ca_file[ARR_SIZE];
    char                crt_file[ARR_SIZE];
    char                key_file[ARR_SIZE];
    char                server_ip[ARR_SIZE];
    int                 server_port;
    int                 broker_port;
    sqlite3             *db;
    struct              mosquitto *mosq;    
    char                mac_address[ARR_SIZE];//the mac address of the host
    char                temp[ARR_SIZE];//temperature
    char                send_buf[ARR_SIZE*6];//the data send to broker
    char                json_data[ARR_SIZE*6];//the created json data
    char                tim[ARR_SIZE*6];//the time now
    char                database_name[ARR_SIZE];
    char                db_filename[ARR_SIZE];
}ctx_t;

#define MESSAGE_COUNT 100000L
#define MESSAGE_SIZE 1024L
#define KEEPALIVE 60
#endif

