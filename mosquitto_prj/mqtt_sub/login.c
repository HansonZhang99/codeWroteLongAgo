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
    char *str=NULL;
    char *ptr=NULL;
    char temp_t[ARR_SIZE]={0};
    float temp;
    int i;
    if(message->payloadlen)
    {
        if(analysis_TLV_msg((char *)(message->payload),message->payloadlen,buf))
        {
            printf("analysis_TLV_msg error\n");
            return ;
        }
        printf("receive the message from topic [%s] : [\n%s\n] \n",((struct login_t *)userdata)->temp_topic,buf);
        if((str=strstr(buf,((struct login_t *)userdata)->temp_topic))!=NULL)
        {
            str+=(strlen(((struct login_t *)userdata)->temp_topic)+1);
            while(*(++str)!='"');
            ptr=str;
            while(*(++ptr)!='"');

            if(((ptr-str)/sizeof(char))>=ARR_SIZE)
                return ;
            memcpy(temp_t,str+1,(ptr-str)/sizeof(char)-1);
            temp_t[(ptr-str)/sizeof(char)-1]='\0';
            temp=atof(temp_t);
            if((int)temp>=34)
            {
                if(mosq_publish(mosq,(struct login_t*)userdata))
                {
                    return ;
                }
                sleep(3);
            }
        }
    }
    return ;
}

struct mosquitto *login_mqtt(struct login_t *login)
{
    struct mosquitto *mosq;
    int loop;
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
        printf("mosquitto_loop_start failure\n");
        return NULL;
    }
    return mosq;
}
