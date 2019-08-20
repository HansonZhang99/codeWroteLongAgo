/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  fifo_usr1.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月11日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月11日 20时47分00秒"
 *                 
 ********************************************************************************/

#include <sys/select.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <libgen.h>
#include <stdlib.h>
#define MAXSIZE 128
#define FIFO_FILE1 "fifo_1"
#define FIFO_FILE2 "fifo_2"



int g_stop;


void sig_pipe(int signum)
{
    if(signum==SIGPIPE)
    {
        printf("read fifo closed,but write fifo still write data to it\n");
        g_stop=1;
    }
}

int main(int c,char **v)
{
    int read_fd,write_fd;
    char buf[MAXSIZE];
    fd_set rd_set;
    int rv;

    signal(SIGPIPE, sig_pipe);
    if(access(FIFO_FILE1,F_OK))
    {
        printf("FIFO file \"%s\" not exist and create it now\n", FIFO_FILE1);
        if(mkfifo(FIFO_FILE1,0666))
        {
            perror("mkfifo");
            return -1;
        }
    }
    if(access(FIFO_FILE2,F_OK))
    {
        printf("FIFO file \"%s\" not exist and create it now\n",FIFO_FILE2);
        if(mkfifo(FIFO_FILE2,0666))
        {
            perror("mkfifo");
            return -1;
        }
    }
    if((read_fd=open(FIFO_FILE1,O_RDONLY))<0)
    {
        perror("open");
        return -1;
    }
    printf("open %s read only success\n",FIFO_FILE1);
    if((write_fd=open(FIFO_FILE2,O_WRONLY))<0)
    {
        perror("open");
        close(read_fd);
        return -1;
    }
    printf("open %s write only success\n",FIFO_FILE2);

    while(!g_stop)
    {
        FD_ZERO(&rd_set);
        FD_SET(STDIN_FILENO,&rd_set);
        FD_SET(read_fd,&rd_set);

        rv=select(read_fd+1,&rd_set,NULL,NULL,NULL);

        if(rv<=0)
        {
            printf("select get timeout or error\n");
            continue;
        }

        if(FD_ISSET(read_fd,&rd_set))
        {
            memset(buf,0,sizeof(buf));
            rv=read(read_fd,buf,sizeof(buf));
            if(rv<0)
            {
                printf("read data from fifo error\n");
                break;
            }
            else if(rv==0)
            {
                printf("fifo closed\n");
                break;
            }
            printf("--->%s",buf);
        }
        if(FD_ISSET(STDIN_FILENO,&rd_set))
        {
            memset(buf,0,sizeof(buf));
            fgets(buf,sizeof(buf),stdin);
            write(write_fd,buf,strlen(buf));
        }
    }
}
