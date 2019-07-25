/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  mosq_publish.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 15时27分37秒"
 *                 
 ********************************************************************************/

#include "mosq_publish.h"

int mosq_publish(struct mosquitto *mosq,struct login_t *login)
{
    if(!mosq||!login)
    {
        printf("Invaild argument mosq or login\n");
        return -1;
    }
    char arr[MAXSIZE];
    unsigned char send_buf[MAXSIZE];

    if(create_json_data(login->mac,"warning",arr,login))
    {
        printf("create json data error\n");
        return -1;
    }
    int num=create_TLV_msg(send_buf,MAXSIZE,TLV_STR,arr,0);
    int i=mosquitto_publish(mosq,NULL,login->warn_topic,num,send_buf,0,0);
    if(i!=0)
    {
        printf("mosquitto_publish failure:%s\n",strerror(errno));
        return -1;
    }
    printf("publish message [\n%s\n] to topic [%s] successfully\n",arr,login->warn_topic);
    return 0;
}
