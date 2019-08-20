/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  shared_mem_write.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月11日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月11日 16时31分50秒"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATH "."
#define PRJID 0x22
#define MAXSIZE 128

struct message
{
    int num;
    char buf[MAXSIZE];
};

int main(int c,char **v)
{
    key_t key;
    int shmid;
    struct message *msg;
    int i;

    if((key=ftok(PATH,PRJID))<0)
    {
        perror("ftok");
        return -1;
    }

    shmid=shmget(key,sizeof(struct message),IPC_CREAT|0666);
    if(shmid<0)
    {
        perror("shget");
        return -1;
    }

    msg=shmat(shmid,NULL,0);

    if((void *)-1==msg)
    {
        perror("shmat");
        return -1;
    }

    msg->num=1;
    snprintf(msg->buf,MAXSIZE,"hello num is %d",1);

    printf("num : %d ,msg : %s\n",msg->num,msg->buf);

    sleep(3);

    for(i=2;i<8;i++)
    {
        msg->num=i;
        snprintf(msg->buf,MAXSIZE,"hello num is %d",i);
        printf("num : %d ,msg : %s\n",msg->num,msg->buf);
        sleep(1);
    }

    shmdt(msg);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
