/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  transport.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/10/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/10/2019 06:53:05 PM"
 *                 
 ********************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"MQTTClient.h"
#include<sys/time.h>
#include<termios.h>
#include<unistd.h>
#include<errno.h>
#define ADDRESS "tcp://127.0.0.1:1883"
#define CLIENTID "ExampleClientPub"
#define TOPIC "test"
#define PAYLOAD "Hello World"
#define QOS 1
#define TIMEOUT 10000L


int main(int argc,char **argv)
{
    MQTTClient                        client;
    MQTTClient_message               *receive_msg=NULL;
    char                             *topic_name=NULL;
    int                               topic_len;
    int                               rv,i;
    MQTTClient_connectOptions         conn_opts=MQTTClient_connectOptions_initializer;
    MQTTClient_message                publish_msg=MQTTClient_message_initializer;
    MQTTClient_deliveryToken          token;
    MQTTClient_create(&client,ADDRESS,CLIENTID,MQTTCLIENT_PERSISTENCE_NONE,NULL);
    
    conn_opts.keepAliveInterval=60;
    conn_opts.cleansession=1;
    printf("try to connect...\n");
    
    if((rv=MQTTClient_connect(client,&conn_opts))!=MQTTCLIENT_SUCCESS)//函数阻塞
    {
        printf("MQTTClient_connect failure:%s\n",strerror(errno));
        return 0;
    }
    publish_msg.payload=PAYLOAD;//可能需要强制类型转换，结构体MQTTClient_message的payload成员使void *类型
    publish_msg.payloadlen=strlen(PAYLOAD);
    publish_msg.qos=QOS;
    publish_msg.retained=0;
    
    MQTTClient_subscribe(client,"MQTT Examples",1);
    
    usleep(10000);
    
    MQTTClient_publishMessage(client,TOPIC,&publish_msg,&token);
    
    printf("Waiting for up to %d seconds for publication of %s\n on topic %s for client with client id[%s]\n",(int)(TIMEOUT/1000),PAYLOAD,TOPIC,CLIENTID);
    
    rv=MQTTClient_waitForCompletion(client,token,TIMEOUT);
    
    printf("Message with delivery token %d delivered\n",token);
      while(1)
    {
        if(MQTTClient_isConnected(client)==1)
        {
            printf("Alive\n");
        }
        else
        {
            printf("Dead \n");
            break;
        }
        if((rv=MQTTClient_receive(client,&topic_name,&topic_len,&receive_msg,5000))!=MQTTCLIENT_SUCCESS)
        {
            printf("MQTTClient_receive failure:%s\n",strerror(errno));
            break;
        }
        printf("Message rev %d delivered\n",rv);
        printf("topic name len:%d\n",topic_len);
        if(topic_name!=NULL)
        {
            printf("topic name:");
            for(i=0;i<topic_len;i++)
            {
                putchar(*topic_name++);
            }
            putchar('\n');
            printf("Data:%s len:%d msgid:%d \n",(char *)receive_msg->payload,receive_msg->payloadlen,receive_msg->msgid);
            if(strcmp((char *)receive_msg->payload,"ESC")==0)
            {
                printf("ESC\n");
                break;
            }

        }
        sleep(3);
    }
    MQTTClient_disconnect(client,10000);
    MQTTClient_destroy(&client);
    return 0;
}
