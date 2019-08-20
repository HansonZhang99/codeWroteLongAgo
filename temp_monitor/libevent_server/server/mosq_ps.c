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

#include "mosq_ps.h"

int mosq_publish(ctx_t *ctx)
{
    if(!ctx)
    {
        printf("Invaild argument ctx\n");
        return -1;
    }
    if(get_temperature(ctx->temp))
    {
        return -1;  
    }
 //   strncpy(ctx->temp,"10",128);
    if(create_json_data(ctx))
    {
        printf("create json data error\n");
        return -1;
    }
    int num=create_TLV_msg(ctx->send_buf,MAXSIZE,TLV_STR,ctx->json_data,0);
    int i=mosquitto_publish(ctx->mosq,NULL,ctx->temp_topic,num,ctx->send_buf,0,0);
    if(i!=0)
    {
        printf("mosquitto_publish failure:%s\n",strerror(errno));
        return -1;
    }
    printf("publish message [\n%s\n] to topic [%s] successfully\n",ctx->json_data,ctx->temp_topic);
    return 0;
}


int mosq_subscribe(ctx_t *ctx)
{
    if(mosquitto_subscribe(ctx->mosq,NULL,ctx->temp_topic,0))
    {
        printf("mosquitto_subscribe error:%s\n",strerror(errno));
        return -1;
    }
    printf("subscribe topic %s successfully\n",ctx->temp_topic);
    mosquitto_loop_forever(ctx->mosq,-1,1);

    return 0;
}


void my_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    printf("connect with the broker successfully\n");
}


void my_disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
    printf("the connection between host and broker has broken,and mqtt program will exit now\n");
}

int login(ctx_t *ctx)
{
    if(!ctx)
    {
        printf("invalid argument ctx\n");
        return -1;
    }
    int loop;

    mosquitto_lib_init();
    ctx->mosq = mosquitto_new(NULL,true,(void *)ctx);
    if(mosquitto_username_pw_set(ctx->mosq,ctx->mqtt_username,ctx->mqtt_password))
    {
        printf("mosquitto_username_pw_set failure:[%d] [%s]\n",errno,strerror(errno));
        return -1;
    }
    mosquitto_connect_callback_set(ctx->mosq,my_connect_callback);
    mosquitto_disconnect_callback_set(ctx->mosq,my_disconnect_callback);
    mosquitto_connect(ctx->mosq,ctx->broker_address,ctx->broker_port,KEEPALIVE);

    loop=mosquitto_loop_start(ctx->mosq);
    if(loop!=MOSQ_ERR_SUCCESS)
    {
        printf("mosquitto_loop_start failure:%s\n",strerror(errno));
        return -1;
    }
    return 0;
}
