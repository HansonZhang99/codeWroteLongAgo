/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  login.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月24日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月24日 13时24分58秒"
 *                 
 ********************************************************************************/

#include "login.h"

void my_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    printf("connected\n");
}


void my_disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
    printf("disconnect\n");
}


void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    char buf[MAXSIZE];
    pthread_t tid1,tid2;
    pthread_attr_t attr1,attr2;
    if(message->payloadlen)
    {
        if(analysis_TLV_msg((unsigned char *)(message->payload),message->payloadlen,buf))
        {
            printf("analysis_TLV_msg error\n");
            return ;
        }
        printf("recieve the message [\n%s\n] form topic %s\n",buf,((struct login_t*)userdata)->warn_topic);
        if(strstr(buf,"warning"))
        {
            printf("recieve the warning,the tempeature is too high!!!\n");
            if(pthread_attr_init(&attr1)<0)
            {
                printf("pthread_attr_init error\n");
                return ;
            }
            if(pthread_attr_setstacksize(&attr1,120*1024)<0)
            {
                printf("pthread_attr_setstacksize error\n");
                return ;
            }
            if(pthread_attr_setdetachstate(&attr1,PTHREAD_CREATE_DETACHED)<0)
            {
                printf("pthread_attr_setstackstate error\n");
                return ;
            }

            if(pthread_attr_init(&attr2)<0)
            {
                printf("pthread_attr_init error\n");
                return ;
            }
            if(pthread_attr_setstacksize(&attr2,120*1024)<0)
            {
                printf("pthread_attr_setstacksize error\n");
                return ;
            }
            if(pthread_attr_setdetachstate(&attr2,PTHREAD_CREATE_DETACHED)<0)
            {
                printf("pthread_attr_setstackstate error\n");
                return ;
            }
            pthread_create(&tid1,&attr1,buzzer,NULL);
            pthread_create(&tid2,&attr2,led_blink,NULL);
            pthread_attr_destroy(&attr1);
            pthread_attr_destroy(&attr2);
        }
    }
    return ;
}

struct mosquitto *login_mqtt(struct login_t *login)
{
    int loop;
    struct mosquitto *mosq;

    mosquitto_lib_init();
    mosq = mosquitto_new(NULL,true,(void *)login);

    if(mosquitto_username_pw_set(mosq,login->username,login->password))
    {
        printf("mosquitto_username_pw_set failure\n");
        return NULL;
    }
    mosquitto_connect_callback_set(mosq,my_connect_callback);
    mosquitto_disconnect_callback_set(mosq,my_disconnect_callback);
    mosquitto_connect(mosq,login->address,login->port,KEEPALIVE);
    mosquitto_message_callback_set(mosq,my_message_callback);

    loop=mosquitto_loop_start(mosq);
    if(loop!=MOSQ_ERR_SUCCESS)
    {
        printf("mosquitto_loop_start failure:%s\n",strerror(errno));
        return NULL;
    }
    return mosq;
}
