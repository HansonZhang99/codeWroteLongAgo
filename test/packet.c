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

#include<stdlib.h>
#include"crc-itu-t.h"
#include<string.h>
#include<stdio.h>
#include<arpa/inet.h>
//unsigned short crc_itu_t(unsigned short crc, const unsigned char *buffer, unsigned int len);
//static inline unsigned short crc_itu_t_byte(unsigned short crc, const unsigned char data)

const unsigned char Head=0xfd;

#define MAXSIZE 64

enum
{
    TLV_STR=1,
    TLV_INT,//includes short and char ,just need more space
    TLV_DOU,
};

/* buf[]:the container of data
 * buf_len:the buf's length
 * Tag:the data type
 * value:the data's first address
 * num:the space which had used in buf
 */
int create_TLV_msg(unsigned char buf[],int buf_len,int Tag,void *value,int num)
{
    if(!buf||!value||Tag>3||Tag<1||num>=buf_len)
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
        buf[num++]=Head;//H
        buf[num++]=Tag;//T
        memcpy(buf+num,&length,sizeof(unsigned char));//L
        num++;
        memcpy(buf+num,ptr,len);//V
        num+=len;
        crc=crc_itu_t(MAGIC_CRC,buf+len_s,num-len_s);
        crc_n=htons(crc);//to big endian
        memcpy(buf+num,&crc_n,sizeof(unsigned short));//C
        printf("%s\n",ptr);
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
}
/* buf:the data's location
 * cmd:the command,1 for byte to short,2 for byte to integer
 */
int byte_to_int(unsigned char *buf,int cmd)
{   //high bytes --------->low bytes
    //for big endian (bytes) to integer or short 
    int val=0;
    int s=0;
    if(!buf)
        return 0;
    if(cmd==1)
    {
        while(s<sizeof(short))
        {
            val+=buf[s++];
            if(s<sizeof(short)) 
                val<<=8;
        }
        return val;
    }
    else if(cmd==2)
    {
        while(s<(sizeof(int)))
        {
            val+=buf[s++];
            if(s<sizeof(int))
            {
                val<<=8;
            }
        }
        return val;
    }
}
/* buf:the locaton of the data
 * len:the data's length(for byte)
 * dst:the container of the translated data
 */
void byte_to_string(unsigned char *buf,int len,char *dst)
{
    if(!buf||len>MAXSIZE||!dst)
        return ;
    memcpy(dst,buf,len);
    dst[len]='\0';
}
/*two bytes to translate,first byte is a integer between -128 to 127,second byte is a decimal between 0  
 * to 1,both have sign*/
double byte_to_double(unsigned char *buf)
{
    double dst;
    if(!buf)
        return 0;
    dst=(char)(*buf)+((double)((char)(*(buf+1))))/100;
    return dst;
}
int analysis_TLV_msg(unsigned char *buf,int rv)
{
    if(!buf||rv>MAXSIZE)
    {
        return 0;
    }
    int i;
    unsigned char *ptr=NULL;
    unsigned short len;
    unsigned short crc;
    unsigned short crc_n;
    unsigned char tag;
    int           value,j;
    unsigned char crc_s[2];
    char          value_s[128];
    for(i=0;i<rv;i++)
    {
        if(buf[i]==0xFD&&i<rv-6)
        {
            ptr=&buf[i];
            tag=*(ptr+1);
            if(tag!=TLV_STR&&tag!=TLV_INT&&tag!=TLV_DOU)
                continue;
            len=*(ptr+2);
            if(len>rv-i)
                continue;
           // printf("%x   %x\n",buf[i+len-2],buf[i+len-1]);
            crc_n=crc_itu_t(MAGIC_CRC,ptr,len-2);//get crc from buf
           // printf("%x   %x\n",((unsigned char *)&crc_n)[1],((unsigned char *)&crc_n)[0]);
            putchar('\n');
            if(((unsigned char *)&crc_n)[1]==buf[i+len-2]&&((unsigned char *)&crc_n)[0]==buf[i+len-1])
            {//compare the two crc bytes whether are all equal or not
                if(tag==TLV_STR)
                {
                    byte_to_string(ptr+3,len-5,value_s);
                    printf("read a string:%s\n",value_s);
                }
                else if(tag==TLV_INT)
                {
                    value=byte_to_int(ptr+3,2);
                    printf("read a integer:%d\n",value);
                }
                else if(tag==TLV_DOU)
                {
                    double a=byte_to_double(ptr+3);
                    printf("read a double:%f\n",a);
                }
            }
        }
    }
    return 1;
}
int main()
{
    unsigned char buf[MAXSIZE];
    memset(buf,0,sizeof(buf));
    int         a=128;
    int         b=-24324;
    char        dst[16];
    int         c=0;
    int         len;
    double      d=-113.4333;
    double      e=12.43;
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Hello",0);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"GOOD",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Apple",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"Open a fo or ,thankycwa",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_INT,&b,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_DOU,&e,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"support",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_STR,"supportss",len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_INT,&a,len);
    len=create_TLV_msg(buf,MAXSIZE,TLV_DOU,&d,len);
    int i;
    for(i=0;i<len;i++)
    {
        printf("0x%02x  ",buf[i]);
        if(buf[i+1]==0xfd)
            putchar('\n');
    }
    putchar('\n');
  /*   char buf_a[]={  0xfd , 0x01,  0x0a , 0x48 , 0x65,  0x6c,  0x6c,  0x6f , 0xa7,  0x6e,  
                    0xfd , 0x01 , 0x09 , 0x47,  0x4f , 0x4f,  0x44 , 0x46 , 0xb6  ,
                    0xfd,  0x01,  0x0a , 0x41,  0x70,  0x70,  0x6c,  0x65,  0x3f,  0xb8,  
                    0xfd,  0x03,  0x07 , 0xf4 , 0xd5,  0xfa,  0x74  };*/
    analysis_TLV_msg(buf,len);

    return 0;
}
