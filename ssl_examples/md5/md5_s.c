/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  md5_s.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/19/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/19/2019 06:01:45 PM"
 *                 
 ********************************************************************************/

#include <openssl/md5.h>
#include <string.h>
#include <stdio.h> 
int main()
{
        MD5_CTX ctx;
        unsigned char outmd[16];
        int i=0;
        memset(outmd,0,sizeof(outmd));
        MD5_Init(&ctx);
        MD5_Update(&ctx,"hello\n",6);
        MD5_Final(outmd,&ctx);
        for(i=0;i<16;i<i++)
        {
                printf("%02X",outmd[i]);
        }
        printf("\n");
        return 0;
}

