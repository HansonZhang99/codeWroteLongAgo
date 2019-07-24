/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  create_json_data.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 12时51分46秒"
 *                 
 ********************************************************************************/

#include "create_json_data.h"

int create_json_data(char *mac,char *temp,char arr[],struct login_t *login)
{
    if(!mac||!temp||!arr)
    {
        printf("Invalid argument mac or temp or arr\n");
        return -1;
    }
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
    cJSON_AddItemToObject(root,"mac address",cJSON_CreateString(mac));
    cJSON_AddItemToObject(root,"topic",item);
    cJSON_AddItemToObject(item,login->temp_topic,cJSON_CreateString(temp));
    strncpy(arr,cJSON_Print(root),MAXSIZE);
    return 0;
}
