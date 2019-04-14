#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include"MQTTClient.h"
#include"set_signal.h"
#include"get_time.h"
#include"get_temperature.h"
#include"opt_init.h"
//此发布端不使用回调函数
int main(int argc,char **argv)
{
	char                      address[128];
	int                       port=0;
	char                      topic[128];
	char                      pub_id[128];
	char                      buf[128]={'\0'};
	char                      date[128];
	char                      address_s[128];
	float                     temper;
	int                       rv;
	const int                 qos=1;
	const long                timeout=10000L;
	if(set_signal()<0)
	{
		printf("set_signal error:%s\n",strerror(errno));
		return -1;
	}
	if(opt_init(&port,address,pub_id,topic,argc,argv)<0)
	{
		printf("opt_init failure:%s\n",strerror(errno));
		return -1;
	}
	snprintf(address_s,sizeof(address_s),"tcp://%s:%d",address,port);
	MQTTClient client;
	MQTTClient_connectOptions conn_opts=MQTTClient_connectOptions_initializer;
	MQTTClient_message publish_msg=MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	conn_opts.keepAliveInterval=60;
	conn_opts.cleansession=1;
	MQTTClient_create(&client,address_s,pub_id,MQTTCLIENT_PERSISTENCE_NONE,NULL);
	if((rv=MQTTClient_connect(client,&conn_opts))!=MQTTCLIENT_SUCCESS)
	{
		printf("MQTTClient_connect error:%s\n",strerror(errno));
		return -1;
	}
	publish_msg.qos=qos;
	publish_msg.retained=0;
	while(!g_stop)
	{
		if(get_time(date)<0)
		{
			printf("get_time error:%s\n",strerror(errno));
			return -1;
		}
		if(get_temperature(&temper)<0)
		{
			printf("get_temperature error:%s\n",strerror(errno));
			return -1;
		}
		snprintf(buf,sizeof(buf),"RPI0001/%s/%f",date,temper);
		publish_msg.payload=(void *)buf;
		publish_msg.payloadlen=strlen(buf);
		MQTTClient_publishMessage(client,topic,&publish_msg,&token);
		printf("Waiting for %d seconds for publication of---- %s---- on topic %s for subscriber with id:%s\n",timeout/1000,buf,topic,pub_id);
		rv=MQTTClient_waitForCompletion(client,token,timeout);
		printf("Message with delivery token %d delivered\n",rv);
		sleep(30);
	}
}
                



