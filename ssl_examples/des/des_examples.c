/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  des_examples.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/19/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/19/2019 09:13:41 PM"
 *                 
 ********************************************************************************/

#include<openssl/des.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>

#define DES_BLOCK_SIZE 8
unsigned char *str2hex(char *str)
{
    unsigned char *ret=NULL;
    int str_len=strlen(str);
    int i=0;
    assert((str_len%2)==0);
    ret=(char *)malloc(str_len/2);
    for(i=0;i<str_len;i+=2)
    {
        sscanf(str+i,"%2hhx",&ret[i/2]);
    }
    return ret;
}
char *padding_buf(char *buf,int size,int *final_size)
{
    char *ret=NULL;
    int pidding_size=DES_BLOCK_SIZE-(size%DES_BLOCK_SIZE);
    int i;
    *final_size=size+pidding_size;
    ret=(char *)malloc(size+pidding_size);
    memcpy(ret,buf,size);
    if(pidding_size!=0)
    {
        for(i=size;i<(size+pidding_size);i++)
        {
            ret[i]=0;
        }
    }
    return ret;
}

void printf_buff(char *buff,int size)
{
    int i=0;
    for(i=0;i<size;i++)
    {
        printf("%02X",(unsigned char )buff[i]);
        if((i+1)%8==0)
            putchar('\n');
    }
    printf("\n\n\n");
}


void encrpyt_buf(char *raw_buf,char **encrpyt_buf,int len)
{
    DES_key_schedule schedule;
    des_cblock *iv3;
    unsigned char *key=str2hex("8cc72b05705d5c46");
    unsigned char *iv=str2hex("667b02a85c61c786");
    iv3=(des_cblock *)malloc(sizeof(unsigned char)*strlen(iv));
    memcpy(iv3,iv,strlen(iv));
    DES_set_key_unchecked((const_DES_cblock *)&key,&schedule);
    DES_cbc_encrypt(raw_buf,*encrpyt_buf,len,&schedule,iv3,DES_ENCRYPT);
    free(key);
    free(iv3);
}


void decrpyt_buf(char *raw_buf,char **encrypt_buf,int len)
{
    DES_key_schedule schedule;
    des_cblock *iv3;
    unsigned char *key=str2hex("8cc72b05705d5c46");
    unsigned char *iv=str2hex("667b02a85c61c786");
    iv3=(des_cblock *)malloc(sizeof(unsigned char)*strlen(iv));
    memcpy(iv3,iv,strlen(iv));
    DES_set_key_unchecked((const_DES_cblock *)&key,&schedule);
    DES_cbc_encrypt(raw_buf,*encrypt_buf,len,&schedule,iv3,DES_DECRYPT);
    free(key);
    free(iv);
    free(iv3);
}



int main(int argc,char **argv)
{
    char *raw_buf=NULL;
    char *after_padding_buf=NULL;
    int padding_size=0;
    char *en_buf=NULL;
    char *de_buf=NULL;

    raw_buf=(char *)malloc(17);
    strncpy(raw_buf,"life's a struggle",17);
    printf("-----------------------------raw_buf\n");
    printf_buff(raw_buf,18);


    after_padding_buf=padding_buf(raw_buf,17,&padding_size);
    printf("-----------------------------after_padding_buf\n");
    printf_buff(after_padding_buf,padding_size);


    en_buf=(char *)malloc(padding_size);
    encrpyt_buf(after_padding_buf,&en_buf,padding_size);
    printf("-----------------------------encrpyt_buf\n");
    printf_buff(en_buf,padding_size);


    de_buf=(char *)malloc(padding_size);
    decrpyt_buf(en_buf,&de_buf,padding_size);
    printf("-----------------------------decrpyt_buf\n");
    printf_buff(de_buf,padding_size);
    printf("%s\n",de_buf);
    free(raw_buf);
    free(after_padding_buf);
    free(en_buf);
    free(de_buf);
    return 0;
}
