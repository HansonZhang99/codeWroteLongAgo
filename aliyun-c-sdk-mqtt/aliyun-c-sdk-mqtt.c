/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *    
 *       Filename:  aliyun-c-sdk-mqtt.c
 *    Description:  This file 
 *                        
 *        Version:  1.0.0(2019年07月18日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月18日 23时45分23秒"
 *                            
 *********************************************************************************/

#include "dev_sign_api.h"
#include "mqtt_api.h"
#include <cJSON.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
//#include <linux/input.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>

#define EV_PRESS 1
#define MAC_FILE "macfile"
#define PATH_1 "/dev/input/event0"
#define PATH_2 "/sys/devices/w1 bus master/"

char DEMO_PRODUCT_KEY[IOTX_PRODUCT_KEY_LEN + 1] = {0};
char DEMO_DEVICE_NAME[IOTX_DEVICE_NAME_LEN + 1] = {0};
char DEMO_DEVICE_SECRET[IOTX_DEVICE_SECRET_LEN + 1] = {0};

void *HAL_Malloc(uint32_t size);
void HAL_Free(void *ptr);
void HAL_Printf(const char *fmt, ...);
int HAL_GetProductKey(char product_key[IOTX_PRODUCT_KEY_LEN + 1]);
int HAL_GetDeviceName(char device_name[IOTX_DEVICE_NAME_LEN + 1]);
int HAL_GetDeviceSecret(char device_secret[IOTX_DEVICE_SECRET_LEN]);
uint64_t HAL_UptimeMs(void);
int HAL_Snprintf(char *str, const int len, const char *fmt, ...);

#define EXAMPLE_TRACE(fmt, ...)  \
    do { \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__); \
        HAL_Printf("%s", "\r\n"); \
    } while(0)

void example_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_t     *topic_info = (iotx_mqtt_topic_info_pt) msg->msg;

    switch (msg->event_type) {
        case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
            /* print topic name and topic message */
            EXAMPLE_TRACE("Message Arrived:");
            EXAMPLE_TRACE("Topic  : %.*s", topic_info->topic_len, topic_info->ptopic);
            EXAMPLE_TRACE("Payload: %.*s", topic_info->payload_len, topic_info->payload);
            EXAMPLE_TRACE("\n");
            break;
        default:
            break;
    }
}

int example_subscribe(void *handle)
{
    int res = 0;
    const char *fmt = "/sys/%s/%s/thing/event/property/post";
    char *topic = NULL;
    int topic_len = 0;

    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    topic = HAL_Malloc(topic_len);
    if (topic == NULL) {
        EXAMPLE_TRACE("memory not enough");
        return -1;
    }
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

    res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS0, example_message_arrive, NULL);
    if (res < 0) {
        EXAMPLE_TRACE("subscribe failed");
        HAL_Free(topic);
        return -1;
    }

    HAL_Free(topic);
    return 0;
}

int get_mac(char mac[])
{
    if(!mac)
        return -1;
    int fd;
    char *ptr="ifconfig eth0 > ";
    char buf[1024]={0};
    snprintf(buf,1024,"%s %s",ptr,MAC_FILE);
    int val=system(buf);
    if((fd=open(MAC_FILE,O_RDWR))<0)
    {
        printf("open failure\n");
        return -1;
    }
    lseek(fd,0,SEEK_SET);
    int rv=read(fd,buf,sizeof(buf));
    if(rv<0)
    {
        printf("read failure\n");
        return -1;
    }
    char *str=strstr(buf,"HWaddr");
    if(!str)
    {
        printf("no mac address\n");
        return -1;
    }
    str+=6;
    while((*str)==' ')
        str++;
    memcpy(mac,str,17);
    mac[17]='\0';
    return 0;
}


int get_temperature(char ch_temp[16])
{
    int fd_temp;
    int result=0;
    float temp;
    DIR *dirp=NULL;
    char *ptr=NULL;
    struct dirent *direntp;
    char path[128]=PATH_2;
    char data[128]={0};
    if((dirp=opendir(PATH_2))==NULL)
    {
        printf("open %s failure\n",PATH_2);
        return -1;
    }
    while((direntp=readdir(dirp))!=NULL)
    {
        if(strstr(direntp->d_name,"28-"))
        {
            strcat(path,direntp->d_name);
            result=1;
        }
    }
    if(!result)
    {
        printf("can not find ds18b20 in %s \n",PATH_2);
        return -1;
    }
    strncat(path,"/w1_slave",sizeof(path));
    if((fd_temp=open(path,O_RDONLY))<0)
    {
        printf("open %s failure\n",path);
        return -1;
    }
    if(read(fd_temp,data,sizeof(data))<0)
    {
        printf("read %s failure\n",path);
        return -1;
    }
    if((ptr=strstr(data,"t="))!=NULL)
    {
        ptr+=2;
        if(!ptr)
        {
            printf("read temperature failure\n");
            return -1;
        }
        temp=atof(ptr)/1000;
        snprintf(ch_temp,16,"%f",temp);
    }
    return 0;
}

int example_publish(void *handle)
{
    int             res = 0;
    const char     *fmt = "/sys/%s/%s/thing/event/property/post";
    char           *topic = NULL;
    int             topic_len = 0;
    char            mac[32]={0};
    char            temp_s[16];
    if(get_mac(mac)<0)
    {
        return -1;
    }
    if(get_temperature(temp_s)<0)
    {
        return -1;
    }
    double temp=atof(temp_s);

    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();

    cJSON_AddItemToObject(root,"method",cJSON_CreateString("thing.service.property.set"));
    cJSON_AddItemToObject(root,"id",cJSON_CreateString(mac));
    cJSON_AddItemToObject(root,"params",item);
    cJSON_AddItemToObject(item,"CurrentTemperature",cJSON_CreateNumber(temp));
    cJSON_AddItemToObject(root,"version",cJSON_CreateString("1.0.0"));
    char           *payload = cJSON_Print(root);

    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    topic = HAL_Malloc(topic_len);
    if (topic == NULL) {
        EXAMPLE_TRACE("memory not enough");
        return -1;
    }
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

    res = IOT_MQTT_Publish_Simple(0, topic, IOTX_MQTT_QOS0, payload, strlen(payload));
    if (res < 0) {
        EXAMPLE_TRACE("publish failed, res = %d", res);
        HAL_Free(topic);
        return -1;
    }

    HAL_Free(topic);
    return 0;
}

void example_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    EXAMPLE_TRACE("msg->event_type : %d", msg->event_type);
}


int main(int argc, char *argv[])
{
    void                   *pclient = NULL;
    int                     res = 0;
    int                     loop_cnt = 0;
    iotx_mqtt_param_t       mqtt_params;

    HAL_GetProductKey(DEMO_PRODUCT_KEY);
    HAL_GetDeviceName(DEMO_DEVICE_NAME);
    HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);

    EXAMPLE_TRACE("ds18b20");

    memset(&mqtt_params, 0x0, sizeof(mqtt_params));

    mqtt_params.handle_event.h_fp = example_event_handle;

    pclient = IOT_MQTT_Construct(&mqtt_params);
    if (NULL == pclient) {
        EXAMPLE_TRACE("MQTT construct failed");
        return -1;
    }

    while (1) {
        if (0 == loop_cnt % 20) {
            example_publish(pclient);
        }

        IOT_MQTT_Yield(pclient, 200);

        loop_cnt += 1;
    }

    return 0;
}

