/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  button_thread.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 13时10分28秒"
 *                 
 ********************************************************************************/

#include "button_thread.h"

void *button_worker(void *args)
{
    if(!args)
        pthread_exit(NULL);
    struct message_a* msg=(struct message_a*)args;
    struct input_event ev_key;

    int fd=open(PATH_1,644);

    if(fd<0)
    {
        printf("open %s failure\n",PATH_1);
        pthread_exit(NULL);
    }
    while(1)
    {
        read(fd,&ev_key,sizeof(struct input_event));
        if(ev_key.type==EV_KEY && ev_key.value==EV_PRESS)
        {
            if(mosq_publish(msg->mosq,msg->login))
            {
                printf("mosq_publish error\n");
                pthread_exit(NULL);
            }
            printf("key %d pressed\n",ev_key.code);
        }
    }
    close(fd);
    return NULL;
}


int button_thread(struct mosquitto *mosq,struct login_t *login)
{
    pthread_t tid;
    pthread_attr_t attr;
    static struct message_a msg;
    msg.mosq=mosq;
    msg.login=login;

    if(pthread_attr_init(&attr)<0)
    {
        printf("pthread_attr_init error\n");
        return -1;
    }
    if(pthread_attr_setstacksize(&attr,120*1024)<0)
    {
        printf("pthread_attr_setstacksize error\n");
        return -1;
    }
    if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)<0)
    {
        printf("pthread_attr_setstackstate error\n");
        return -1;
    }
    pthread_create(&tid,&attr,button_worker,&msg);
    pthread_attr_destroy(&attr);
    return 0;
}
