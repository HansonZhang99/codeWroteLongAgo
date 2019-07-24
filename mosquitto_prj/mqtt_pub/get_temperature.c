/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  get_temperature.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 12时39分43秒"
 *                 
 ********************************************************************************/


/*-------------------------------------------
 * Description:
 * this function get the device's temperature
 * and save it to the array user exports.
 * Return value:
 * 0:successful
 * !0:error
 * -----------------------------------------*/

#include "get_temperature.h"

int get_temperature(char ch_temp[16])
{
    if(!ch_temp)
    {
        printf("Invalid argument temp\n");
        return -1;
    }
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
    close(fd_temp);
    return 0;
}
