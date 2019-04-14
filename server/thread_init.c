#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<errno.h>
#include"thread_init.h"
int thread_init(pthread_t tid,void *(*function)(void *),void *fd)
{
        pthread_attr_t      attr;
        if(pthread_attr_init(&attr))
        {
                printf("pthread_attr_init failure:%s\n",strerror(errno));
                pthread_attr_destroy(&attr);
                return -1;
        }
        if(pthread_attr_setstacksize(&attr,128*1024))
        {
                printf("pthread_attr_setsatcksize failure:%s\n",strerror(errno));
                pthread_attr_destroy(&attr);
                return -1;
        }
        if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED))
        {
                printf("pthread_attr_setstackstate failure:%s\n",strerror(errno));
                pthread_attr_destroy(&attr);
                return -1;
        }
        if(pthread_create(&tid,&attr,function,fd))
        {
                printf("pthread_create failure:%s\n",strerror(errno));
                pthread_attr_destroy(&attr);
                return -1;
        }
}
        

