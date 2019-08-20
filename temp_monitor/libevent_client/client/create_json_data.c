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

int create_json_data(ctx_t *ctx)
{
    if(!ctx)
    {
        printf("Invalid argument ctx\n");
        return -1;
    }
    get_time(ctx->tim);
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
    cJSON_AddItemToObject(root,"mac address",cJSON_CreateString(ctx->mac_address));
    cJSON_AddItemToObject(root,"time",cJSON_CreateString(ctx->tim));
    cJSON_AddItemToObject(root,"topic",item);
    cJSON_AddItemToObject(item,ctx->temp_topic,cJSON_CreateString(ctx->temp));
    strncpy(ctx->json_data,cJSON_Print(root),ARR_SIZE*6);
    return 0;
}
