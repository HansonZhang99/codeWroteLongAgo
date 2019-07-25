/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  mosq_subscribe.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2019年07月23日)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "2019年07月23日 16时29分13秒"
 *                 
 ********************************************************************************/

#include "mosq_subscribe.h"

int mosq_subscribe(struct mosquitto *mosq,struct login_t *login)
{
    if(mosquitto_subscribe(mosq,NULL,login->warn_topic,0))
    {
        printf("mosquitto_subscribe error:%s\n",strerror(errno));
        return -1;
    }
    printf("subscribe topic %s successfully\n",login->warn_topic);
    mosquitto_loop_forever(mosq,-1,1);

    return 0;
}
