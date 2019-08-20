/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  test.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年08月10日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年08月10日 23时07分15秒"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define FTOK_PATH "."
#define FTOK_PROJID 0x22
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};
int semaphore_init(void);
int semaphore_p(int semid,int which);
int semaphore_v(int semid,int which);
void semaphore_term(int semid);


int main(int argc, char **argv)
{
    int semid;
    pid_t pid;
    int i;
    if((semid=semaphore_init()) < 0)
    {
        printf("semaphore initial failure: %s\n", strerror(errno));
        return -1;
    }
    printf("before P 0\n");
    semaphore_p(semid,0);
    printf("after P 0\n");
    printf("after 3 seconds,I will V 1\n");
    sleep(3);
    semaphore_v(semid,1);
    printf("after V 1\n");
    semaphore_term(semid);
    return 0;
}

int semaphore_init(void)
{
    key_t key;
    int semid;
    union semun sem_union_1;
    union semun sem_union_2;
    if( (key=ftok(FTOK_PATH, FTOK_PROJID)) < 0 )
    { 
        printf("ftok() get IPC token failure: %s\n", strerror(errno));
        return -1;
    }
    semid = semget(key, 2, IPC_CREAT|0644);
    if( semid < 0)
    {
        printf("semget() get semid failure: %s\n", strerror(errno));
        return -2;
    }
    sem_union_1.val = 0;
    sem_union_2.val = 0;
    if( semctl(semid, 0, SETVAL, sem_union_1)<0 )
    {
        printf("semctl() set initial value failure: %s\n",                
                strerror(errno));
        return -3;
    }  
    if( semctl(semid, 1, SETVAL, sem_union_2)<0 )
    {
        printf("semctl() set initial value failure: %s\n", strerror(errno));
        return -3;
    }
    printf("Semaphore get key_t[0x%x] and semid[%d]\n", key, semid);
    return semid;
}
void semaphore_term(int semid)
{
    union semun sem_union;
    if( semctl(semid, 0, IPC_RMID,sem_union)<0 )
    {
        printf("semctl() delete semaphore ID 0 failure: %s\n",
                strerror(errno));
    }
    return ;
}
int semaphore_p(int semid,int which)
{
    struct sembuf _sembuf;
    _sembuf.sem_num = which;
    _sembuf.sem_op = -1;
    _sembuf.sem_flg = SEM_UNDO; // IPC_NOWAIT SEM_UNDO
    if( semop(semid, &_sembuf, 1) < 0 )
    {
        printf("semop P operator failure: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}
int semaphore_v(int semid,int which)
{
    struct sembuf _sembuf;
    _sembuf.sem_num = which;
    _sembuf.sem_op = 1;
    _sembuf.sem_flg = SEM_UNDO; // IPC_NOWAIT SEM_UNDO
    if( semop(semid, &_sembuf, 1) < 0 )
    {
        printf("semop V operator failure: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}
