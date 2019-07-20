/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  vvv.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/11/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/11/2019 09:11:11 PM"
 *                 
 ********************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include"MQTTClient.h"

int main()
{
    char *address="tcp://localhost:1883";
    char *client_id="client_sub";
    char *payload="mqtt_examples";
    int    rv,i;
    char *ptr=NULL;
    char *topic=NULL;
    int topic_len;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts=MQTTClient_connectOptions_initializer;
    MQTTClient_deliveryToken token;
    MQTTClient_message *receive_msg=NULL;
    //memset(receive_msg,0,sizeof(receive_msg));segmetation fault
    //printf("%ld\n",sizeof(receive_msg));
    conn_opts.keepAliveInterval=60;
    conn_opts.cleansession=1;

    if((rv=MQTTClient_create(&client,address,client_id,MQTTCLIENT_PERSISTENCE_NONE,NULL))<0)
    {
        printf("MQTTClient_create failure:%s\n",strerror(errno));
        return 0;
    }
    printf("MQTTClient_create successfully\n");
    if((rv=MQTTClient_connect(client,&conn_opts))!=MQTTCLIENT_SUCCESS)
    {
        printf("MQTTClient_connect failure:%s\n",strerror(errno));
        return 0;
    }
    printf("MQTTClient_connect successfuly\n");
    MQTTClient_subscribe(client,payload,1);
    /* if((rv=MQTTClient_receive(client,&topic,&topic_len,&receive_msg,5000))!=MQTTCLIENT_SUCCESS)
    { 
        printf("MQTTClient_receive failure:%s\n",strerror(errno));
        return 0;
    }  
    printf("MQTTClient_receive successfully\n");*/
    //receive 函数放在外面传递信息不会改变
    while(1)
    {
        if((rv=MQTTClient_receive(client,&topic,&topic_len,&receive_msg,100000))!=MQTTCLIENT_SUCCESS)//最后一个参数是超时时间,单位是毫秒
        {
            printf("MQTTClient_receive failure:%s\n",strerror(errno));
            break;
        }
        printf("MQTTClient_receive successfully\n");
        ptr=receive_msg->payload;
    //    if(MQTTClient_isConnected(client)!=1)
      //      break;
        printf("Topic:%s\nTopic_len:%d\nmsg:",topic,topic_len);
        for(i=0;i<receive_msg->payloadlen;i++)
        {
            putchar(*ptr++);
        }
        printf("\nmsg_len:%d\nmsg_id:%d\n",receive_msg->payloadlen,receive_msg->msgid);
        sleep(3);

    }
    //free(topic);
    //free(receive_msg);
    printf("end\n");
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
    

}
