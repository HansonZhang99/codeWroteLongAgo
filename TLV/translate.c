/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  analysis_s.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/04/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "03/04/19 14:17:49"
 *                 
 ********************************************************************************/
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<errno.h>
#include"analysis.h"
#include<unistd.h>
#include"translate.h"
#include"crc-itu-t.h"
int translate(int fd)
{
    if(!fd)
        return -1;
    int bytes=0;
    unsigned char buf_s[MAXSIZE]={'\0'};
    unsigned char buf[MAXSIZE*4]={'\0'};
    int rv=0;
    int i;
    char value_s[MAXSIZE*4]={'\0'};
    int value_i=0;
    double value_d=0.00;
    unsigned short length=0;
    short crc;
    while(1)
    {
        rv=read(fd,buf_s,MAXSIZE);
        if(bytes+rv>MAXSIZE*4)
        {
            printf("This data is to large,we have to throw it\n");
            memset(buf,0,sizeof(buf));
            bytes=0;
            continue;
        }
        if(rv<=0)
        {
            for(i=0;i<bytes;i++)
            {
                printf("0x%02x ",buf[i]);
            }
            printf("Use function read error:%s\n",strerror(errno));
            return -1;
        }
        printf("Read a buffer:%d bytes data\n",rv);
        memcpy(&buf[bytes],buf_s,rv);
        for(i=0;i<bytes+rv;i++)
        {
            if(buf[i]==0xFD)//search head(0xfd) in buffer
            {
                if(rv+bytes-i<6)//find 0xfd in buffer's last five bytes,a complete data packet have at least 6 bytes(1 Head + 1 tag +1 length +2 crc + value(>=1))
                {
                    memset(buf,0,i);
                    memmove(buf,&buf[i],rv+bytes-i);
                    bytes=rv+bytes-i;
                    break;//goto
                }
                if(buf[i+1]==TLV_STR)
                {
                    if(buf[i+2]<6)//a string has at least 6 bytes
                        continue;
                    if(buf[i+2]>(rv+bytes-i))//a string packet can be very big,this packet hasn't tranported completely
                    {
                        memset(buf,0,i);
                        memmove(buf,&buf[i],rv+bytes-i);
                        bytes=rv+bytes-i;
                        break;
                    }
                    length=buf[i+2];
                    crc=crc_itu_t(MAGIC_CRC,&buf[i],length-2);
                    if(((unsigned char *)&crc)[1]==buf[i+length-2]&&((unsigned char *)&crc)[0]==buf[i+length-1])//crc pairing successful
                    {
                        byte_to_string(&buf[i+3],length-5,value_s);
                        printf("Read a string++++:  %s\n",value_s);
                        i+=(length-1);
                        continue;
                    }
                }
                else if(buf[i+1]==TLV_CHR)
                {
                    if(buf[i+2]<6)
                        continue;
                    if(buf[i+2]>(rv+bytes-i))
                    {
                         memset(buf,0,i);
                         memmove(buf,&buf[i],rv+bytes-i);
                         bytes=rv+bytes-i;
                         break;
                    }
                    length=buf[i+2];
                    if(length!=6)
                        continue;
                    crc=crc_itu_t(MAGIC_CRC,&buf[i],length-2);
                    if(((unsigned char *)&crc)[1]==buf[i+length-2]&&((unsigned char *)&crc)[0]==buf[i+length-1])//crc pairing successful
                    { 
                        value_i=byte_to_int(&buf[i+3],3);
                        printf("Read a char++++++:  %d\n",value_i);
                        i+=(length-1);
                        continue; 
                    }
                }
                else if(buf[i+1]==TLV_SHT)
                {
                    if(buf[i+2]<6) 
                        continue; 
                    if(buf[i+2]>(rv+bytes-i))
                    {
                        memset(buf,0,i);
                        memmove(buf,&buf[i],rv+bytes-i);
                        bytes=rv+bytes-i;
                        break;
                    }
                    length=buf[i+2];
                    if(length!=7)
                        continue;
                    crc=crc_itu_t(MAGIC_CRC,&buf[i],length-2);
                    if(((unsigned char *)&crc)[1]==buf[i+length-2]&&((unsigned char *)&crc)[0]==buf[i+length-1])//crc pairing successful
                    {
                        value_i=byte_to_int(&buf[i+3],1);
                        printf("Read a short+++++:  %d\n",value_i);
                        i+=(length-1);
                        continue;
                    }
                }
                else if(buf[i+1]==TLV_INT)
                {
                    if(buf[i+2]<9)
                        continue;
                    if(buf[i+2]>(rv+bytes-i))
                    {
                        memset(buf,0,i);
                        memmove(buf,&buf[i],rv+bytes-i);
                        bytes=rv+bytes-i;
                        break;
                    }
                    length=buf[i+2];
                    if(length!=9)
                        continue;
                    crc=crc_itu_t(MAGIC_CRC,&buf[i],length-2);
                    if(((unsigned char *)&crc)[1]==buf[i+length-2]&&((unsigned char*)&crc)[0]==buf[i+length-1])
                    {
                        value_i=byte_to_int(&buf[i+3],2);
                        printf("read a integer++++:  %d\n",value_i);
                        i+=(length-1);
                        continue;
                    }
                }
                else if(buf[i+1]==TLV_DOU)
                {
                    if(buf[i+2]<7)
                        continue;
                    if(buf[i+2]>(rv+bytes-i))
                    {
                        memset(buf,0,i);
                        memmove(buf,&buf[i],rv+bytes-i);
                        bytes=rv+bytes-i;
                        break;
                    }
                    length=buf[i+2];
                    if(length!=7)
                        continue;
                    crc=crc_itu_t(MAGIC_CRC,&buf[i],length-2);
                    if(((unsigned char *)&crc)[1]==buf[i+length-2]&&((unsigned char *)&crc)[0]==buf[i+length-1])
                    {
                        value_d=byte_to_double(&buf[i+3]);
                        printf("Read a double++++:  %f\n",value_d);
                        i+=(length-1);
                        continue;
                    }
                }
            }     
        }
    }
    return 0;
}
