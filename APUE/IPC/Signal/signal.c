/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  signal.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月12日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月12日 15时36分31秒"
 *                 
 ********************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
int g_flag1,g_flag2;
#if 0
void sig_fun1(int signum)
{
    if(signum==SIGUSR1)
    {
        g_flag1=1;
    }
}
#endif
void sig_fun2(int signum)
{
    if(signum=SIGUSR2)
    {
        g_flag2=1;
    }
}
int main(int c,char **v)
{
    int status;
    int flag=0;
    pid_t pid;
    pid=fork();
//    signal(SIGUSR1,sig_fun1);
    signal(SIGUSR2,sig_fun2);
    if(pid<0)
    {
        printf("fork failure:%s\n",strerror(errno));
        return -1;
    }
    else if(pid==0)
    {
        sleep(3);
        printf("send a SIGUSR2 signal to the parent process\n");
        kill(getppid(),SIGUSR2);
        sleep(1);
        printf("child process is running,and flag2 is %d\n",g_flag2);
        while(!g_flag2);
        printf("child process exit while,and flag2 is %d\n",g_flag2);
        return 0;
    }
    printf("parent process is running,and flag2 is %d\n",g_flag2);
    while(!g_flag2);
    printf("parent process exit while,and flag2 is %d\n",g_flag2);
    sleep(3);
    printf("send a SIGUSR2 signal to the child process\n");
    kill(pid,SIGUSR2);
    wait(&status);
    printf("every process have exit\n");
}
