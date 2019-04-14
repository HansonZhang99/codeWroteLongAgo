/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  rsa_examples.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/20/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/20/2019 02:45:05 PM"
 *                 
 ********************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#define OPENSSLKEY "test.key"
#define PUBLICKEY "test_pub.key"
char *encrypt(char *str,char *path_key)
{
    char *p_en=NULL;
    RSA *p_rsa=NULL;
    FILE *fp=NULL;
    int rsa_len=0;

    if((fp=fopen(path_key,"rb"))==NULL)
    {
        printf("open %s failure\n",path_key);
        goto clean;
    }
    if((p_rsa=PEM_read_RSA_PUBKEY(fp,NULL,NULL,NULL))==NULL)
    {
        ERR_print_errors_fp(stdout);
        goto clean;
    }
    rsa_len=RSA_size(p_rsa);

    p_en=(char *)malloc(rsa_len+1);
    if(!p_en)
    {
        printf("malloc failure\n");
        goto clean;
    }
    memset(p_en,0,rsa_len+1);

    if(RSA_public_encrypt(rsa_len,(unsigned char *)str,(unsigned char *)p_en,p_rsa,RSA_NO_PADDING)<0)
    {
        printf("RSA_public_encrypt error\n");
        goto clean;
    }

clean:
    if(p_rsa)    RSA_free(p_rsa);
    if(fp)       fclose(fp);
    
    return p_en;
}

char *decrypt(char *str,char *path_key)
{
    char *p_de=NULL;
    RSA *p_rsa=NULL;
    FILE *fp=NULL;
    int rsa_len=0;//initialize to zero
    
    if((fp=fopen(path_key,"rb"))==NULL)
    {
        printf("open %s error\n",path_key);
        goto clean;
    }

    if((p_rsa=PEM_read_RSAPrivateKey(fp,NULL,NULL,NULL))==NULL)
    {
        ERR_print_errors_fp(stdout);
        goto clean;
    }

    rsa_len=RSA_size(p_rsa);

    p_de=(char *)malloc(rsa_len+1);

    if(!p_de)
    {
        printf("malloc error\n");
        goto clean;
    }
    memset(p_de,0,rsa_len+1);

    if(RSA_private_decrypt(rsa_len,(unsigned char *)str,(unsigned char *)p_de,p_rsa,RSA_NO_PADDING)<0)
    {
        printf("RSA_private_decrypt error\n");
        goto clean;
    }
clean:
    if(p_rsa)    RSA_free(p_rsa);
    if(fp)       fclose(fp);
    
    return p_de;
}



int main(int argc,char **argv)
{
    char *source="i like dancing !!!";
    char *ptf_en;
    char *ptf_de;
    printf("source is :     %s\n",source);

    ptf_en=encrypt(source,PUBLICKEY);
    printf("pth_ed is :     %s\n",ptf_en);

    ptf_de=decrypt(ptf_en,OPENSSLKEY);
    printf("ptf_de is :     %s\n",ptf_de);

    free(ptf_en);
    free(ptf_de);
    return 0;
}
