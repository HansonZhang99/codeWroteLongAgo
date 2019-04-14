#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include"MQTTClient.h"
#include<unistd.h>
#include"set_signal.h"
#include"opt_init.h"
#include"db_init.h"
#include"insert_db.h"
#include"string_break.h"
volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context,MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n",dt);
	deliveredtoken=dt;
}

int msgarrvd(void *context,char *topicName,int topicLen,MQTTClient_message *message)
{
	int i;
	char buf[128]={'\0'};
	char id[128];
	char date[128];
	char temper[128];
	char *payloadptr;
	payloadptr=message->payload;
	printf("Message arrived \n");
	printf("topic:%s\n",topicName);
	printf("Message:%s\n",message->payload);
	printf("message:");
	for(i=0;i<message->payloadlen;i++)
	{
		buf[i]=*payloadptr;
		putchar(*payloadptr++);
	}
	printf("%s\n",buf);
	putchar('\n');
	if(string_break(id,date,temper,buf)<0)
	{
		printf("string_break failure:%s\n",strerror(errno));
		return -1;
	}
	if(db_init("Messages","message")<0)
	{
		printf("db_init failure:%s\n",strerror(errno));
		return -1;
	}
	if(insert_db("Messages","message",id,date,temper)<0)
	{
		printf("insert_db failure:%s\n",strerror(errno));
		return -1;
	}
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void connlost(void *context,char *cause)
{
	printf("Connection lost\n");
	printf("cause %s\n",cause);
}


int main(int argc,char **argv)
{
	MQTTClient client;
	const int qos=1;
        const long timeout=10000L;
	char buf[128];
	int port=0;
	char address[128];
	char sub_id[128];
	char topic[128];
	MQTTClient_connectOptions conn_opts=MQTTClient_connectOptions_initializer;
	int rc,ch;
	if(set_signal()<0)
	{
		printf("set_signal failure:%s\n",strerror(errno));
		return -1;
	}
	if(opt_init(&port,address,sub_id,topic,argc,argv)<0)
	{
		printf("opt_init failure:%s\n",strerror(errno));
		return 1;
	}
	snprintf(buf,sizeof(buf),"tcp://%s:%d",address,port);
	MQTTClient_create(&client,buf,sub_id,MQTTCLIENT_PERSISTENCE_NONE,NULL);
	conn_opts.keepAliveInterval=20;
	conn_opts.cleansession=1;
	MQTTClient_setCallbacks(client,NULL,connlost,msgarrvd,delivered);

	if((rc=MQTTClient_connect(client,&conn_opts))!=MQTTCLIENT_SUCCESS)
	{
		printf("MQTTClient_connect failure:%s\n",strerror(errno));
		return -1;
	}
	printf("Subscribe to topic %s for client %s using QOS %d\n",topic,sub_id,qos);
	MQTTClient_subscribe(client,topic,qos);
	do
	{
		ch=getchar();
	}while(ch!='q'&&ch!='Q');
	MQTTClient_disconnect(client,10000);
	MQTTClient_destroy(&client);
	return rc;
}












