#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#if !defined(WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif

#define NUM_THREADS 2
#define ADDRESS     "tcp://localhost:1883" //���Ĵ˴���ַ
#define CLIENTID    "pub" //���Ĵ˴��ͻ���ID
#define SUB_CLIENTID    "sub" //���Ĵ˴��ͻ���ID
#define TOPIC       "topic01"  //���ķ��͵Ļ���
#define PAYLOAD     "Hello Man, Can you see me ?!" //
#define QOS         1
#define TIMEOUT     10000L
#define USERNAME    "test_user"
#define PASSWORD    "jim777"
#define DISCONNECT  "out"

int CONNECT = 1;
volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;

    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");

    payloadptr = message->payload;
    if(strcmp(payloadptr, DISCONNECT) == 0){
        printf(" \n out!!");
        CONNECT = 0;
    }
    
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    printf("\n");
    
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

void *subClient(void *threadid){
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, SUB_CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
   // conn_opts.username = USERNAME;
    //conn_opts.password = PASSWORD;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);

    do 
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    
    MQTTClient_unsubscribe(client, TOPIC);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
   
   pthread_exit(NULL);
}
void *pubClient(void *threadid){
   long tid;
   tid = (long)threadid;
   int count = 0;
   printf("Hello World! It's me, thread #%ld!\n", tid);
//����һ��MQTTClient
    MQTTClient client;
    //��ʼ��MQTT Clientѡ��
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    //#define MQTTClient_message_initializer { {'M', 'Q', 'T', 'M'}, 0, 0, NULL, 0, 0, 0, 0 }
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    //������Ϣtoken
    MQTTClient_deliveryToken token;
    int rc;
    //ʹ�ò�������һ��client�������丳ֵ��֮ǰ������client
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    //conn_opts.username = USERNAME;
    //conn_opts.password = PASSWORD;
     //ʹ��MQTTClient_connect��client���ӵ���������ʹ��ָ��������ѡ��ɹ��򷵻�MQTTCLIENT_SUCCESS
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    while(CONNECT){
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
    usleep(3000000L);
    }
    
    
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}
int main(int argc, char* argv[])
{
    pthread_t threads[NUM_THREADS];
    long t;
    pthread_create(&threads[0], NULL, subClient, (void *)0);
    pthread_create(&threads[1], NULL, pubClient, (void *)1);
    pthread_exit(NULL);
}
