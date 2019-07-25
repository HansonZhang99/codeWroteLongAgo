/********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  create_json_data.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 12时56分42秒"
 *                 
 ********************************************************************************/

#ifndef __CREATE_JSON_DATA
#define __CREATE_JSON_DATA

#include "cJSON.h"
#include <string.h>
#include "analysis.h"
#include <stdio.h>
#include "login.h"
#include "get_time.h"
int create_json_data(char *mac,char *temp,char arr[],struct login_t * login);

#endif
