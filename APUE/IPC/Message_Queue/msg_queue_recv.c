/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  msg_queue_send.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月11日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月11日 19时24分01秒"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define PATH "."
#define PRJID 0x22
#define MAXSIZE 128

struct message
{      
    long type;
    char buf[MAXSIZE];
};     

int main(int c,char **v)
{      
    key_t key;
    int msgid;
    struct message msg;
    int msgtype;
    int i;
    if((key=ftok(PATH,PRJID))<0)
    {  
        perror("ftok");
        return -1;
    }  

    msgid=msgget(key,IPC_CREAT|0666);
    if(msgid<0)
    {  
        perror("msgget");
        return -1;
    }  
    memset(&msg,0,sizeof(msg));
    msgtype=1;
    msg.type=msgtype;
    for(i=0;i<4;i++)
    {  

        if(msgrcv(msgid,&msg,sizeof(msg.buf),msgtype,IPC_NOWAIT)<0)
        {
            perror("msgrcv");
            return -1;
        }
        printf("receive message : %s\n",msg.buf);
        sleep(1);

    }
    msgctl(msgid,IPC_RMID,NULL);
    return 0;
}
