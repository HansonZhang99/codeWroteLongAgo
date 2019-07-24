/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  get_temperature.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 12时42分41秒"
 *                 
 ********************************************************************************/

#ifndef __GET_TEMPETATURE__
#define __GET_TEMPETATURE__

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PATH_2 "/sys/devices/w1 bus master/"
int get_temperature(char ch_temp[]);

#endif

