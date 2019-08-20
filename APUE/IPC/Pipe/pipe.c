/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  pipe.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月12日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月12日 17时15分21秒"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXSIZE 128
#define MSG "hello,this is parent process"
int main()
{
    int pipe_fd[2];
    char buf[MAXSIZE];
    pid_t pid;
    int status;
    int rv;
    if(pipe(pipe_fd)<0)
    {
        printf("pipe failure:%s\n",strerror(errno));
        return -1;
    }
    pid=fork();
    if(pid<0)
    {
        printf("fork failure:%s\n",strerror(errno));
        return -1;
    }
    else if(pid==0)
    {
        close(pipe_fd[1]);
        memset(buf,0,sizeof(buf));
        rv=read(pipe_fd[0],buf,sizeof(buf));
        if(rv<0)
        {
            printf("read failure:%s\n",strerror(errno));
            return -1;
        }
        printf("read %d bytes data from parent process: %s\n",rv,buf);
        return 0;
    }
    else
    {
        close(pipe_fd[0]);
        if(write(pipe_fd[1],MSG,sizeof(MSG))<0)
        {
            printf("wirte failure:%s\n",strerror(errno));
            return -1;
        }
        printf("write %d bytes data to child process :%s\n",strlen(MSG)+1,MSG);
        wait(&status);
        return 0;
    }
}
