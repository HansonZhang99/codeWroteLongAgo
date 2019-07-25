/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  packet.c
 *    Description:  This file 
 *            
 *        Version:  1.0.0(03/30/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/30/2019 01:53:30 PM"
 *                 
 ********************************************************************************/
#include"packet.h"
int create_TLV_msg(unsigned char buf[],int buf_len,int Tag,void *value,int num)
{
    if(!buf||!value||Tag>5||Tag<1||num>=buf_len)
        return num;
    unsigned char  length;
    unsigned short crc,crc_n;
    int            len;
    int            len_s=num;
    if(Tag==TLV_STR)
    {
        char *ptr=(char *)value;
        len=strlen(value);
        length=sizeof(unsigned char)+sizeof(unsigned char)+len+sizeof(unsigned short)+sizeof(unsigned char);//length=H+T+L+V+CRC
        if((buf_len-num-length)<0)//a shortcoming of this function,if the data is too long,this buf will not to accept this data
        {
            return num;
        }
       // printf("%d\n",len);
        buf[num++]=Head;//H
        buf[num++]=Tag;//T
        memcpy(buf+num,&length,sizeof(unsigned char));//L
        num++;
        memcpy(buf+num,ptr,len);//V
        num+=len;
        crc=crc_itu_t(MAGIC_CRC,buf+len_s,num-len_s);
        crc_n=htons(crc);//to big endian
        memcpy(buf+num,&crc_n,sizeof(unsigned short));//C
        return num+=2;
    }
    else if(Tag==TLV_CHR)
    {
        unsigned char *ptr=(unsigned char *)value;
        len=sizeof(unsigned char );
        length=sizeof(unsigned char )+len+sizeof(unsigned short)+sizeof(unsigned char)+sizeof(unsigned char);
        if((buf_len-num-length)<0)
        {
            return num;
        }
        buf[num++]=Head;
        buf[num++]=Tag;
        memcpy(buf+num,&length,sizeof(unsigned char));
        num++;
        memcpy(buf+num,ptr,len);
        num+=len;
        crc=crc_itu_t(MAGIC_CRC,buf+len_s,num-len_s);
        crc_n=htons(crc);
        memcpy(buf+num,&crc_n,sizeof(unsigned short));
        return num+=2;
    }
    else if(Tag==TLV_SHT)
    {
        short val=(htons)(*(short *)value);
        short *ptr=&val;
        len=sizeof(short);
        length=sizeof(unsigned char)+sizeof(unsigned char)+len+sizeof(unsigned short)+sizeof(unsigned char);//length=H+T+L+V+CRC
        if((buf_len-num-length)<0)
        {   
            return num;
        }   
        buf[num++]=Head;//H
        buf[num++]=Tag;//T
        memcpy(buf+num,&length,sizeof(unsigned char));//L
        num++;
        memcpy(buf+num,ptr,len);//V
        num+=len;
        crc=crc_itu_t(MAGIC_CRC,buf+len_s,num-len_s);
        crc_n=htons(crc);//to big endian
        memcpy(buf+num,&crc_n,sizeof(unsigned short));//C
        return num+=2;
    }
    else if(Tag==TLV_INT)
    {
        int val=(htonl)(*(int *)value);
        int *ptr=&val;
        len=sizeof(int);
        length=sizeof(unsigned char)+sizeof(unsigned char)+len+sizeof(unsigned short)+sizeof(unsigned char);//length=H+T+L+V+CRC
        if((buf_len-num-length)<0)
        {
            return num;
        }
        buf[num++]=Head;//H
        buf[num++]=Tag;//T
        memcpy(buf+num,&length,sizeof(unsigned char));//L
        num++;
        memcpy(buf+num,ptr,len);//V
        num+=len;
        crc=crc_itu_t(MAGIC_CRC,buf+len_s,num-len_s);
        crc_n=htons(crc);//to big endian
        memcpy(buf+num,&crc_n,sizeof(unsigned short));//C
        return num+=2;
    }
    else if(Tag==TLV_DOU)
    {
        char ptr=*(double *)value;
        char str=(((int)((*(double *)value)*100))%100);
        len=sizeof(unsigned char )*2;
        length=sizeof(unsigned char)+sizeof(unsigned char)+len+sizeof(unsigned short)+sizeof(unsigned char);//length=H+T+L+V+CRC
        if((buf_len-num-length)<0)
        {
            return num;
        }
        buf[num++]=Head;//H
        buf[num++]=Tag;//T
        memcpy(buf+num,&length,sizeof(unsigned char));//L
        num++;
        memcpy(buf+num,&ptr,sizeof(unsigned char));//V
        num++;
        memcpy(buf+num,&str,sizeof(unsigned char));//V
        num++;
        crc=crc_itu_t(MAGIC_CRC,buf+len_s,num-len_s);
        crc_n=htons(crc);//to big endian(net endian)
        memcpy(buf+num,&crc_n,sizeof(unsigned short));//C
        return num+=2;
    }
    return num;
}
