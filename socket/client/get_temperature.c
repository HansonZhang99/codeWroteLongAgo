#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include"get_temperature.h"
int  get_temperature(float *temper)
{
        char    path[128]="/sys/bus/w1/devices/";
        char    path_s[32];
        char    buf[128];
        char    *ptr;
        DIR     *dirp;
        int     a=0;
        int     fd;
        struct dirent *direntp;
        if((dirp=opendir(path))==NULL)
        {
                printf("Open %s failure:%s\n",path,strerror(errno));
                return -1;
        }
        while((direntp=readdir(dirp))!=NULL)
        {
                if(strstr(direntp->d_name,"28-"))
                {
                        strcpy(path_s,direntp->d_name);
                        a=1;
                }
        }
        if(a==0)
        {
                printf("Can not find ds18b20 in %s\n",path);
                return -1;
        }
        strncat(path,path_s,sizeof(path));
        strncat(path,"/w1_slave",sizeof(path));
        if((fd=open(path,O_RDONLY))<0)
        {
                printf("Open %s failure:%s\n",path,strerror(errno));
                return -1;
        }
        if(read(fd,buf,sizeof(buf))<0)
        {
                printf("read data from %s failure:%s\n",path,strerror(errno));
                return -1;
        }
        ptr=strstr(buf,"t=");

        ptr+=2;

        if(!ptr)
        {
                printf("ERROR:%s\n",strerror(errno));
                return 1000;
        }
        *temper=atof(ptr)/1000;
        return 0;
}

