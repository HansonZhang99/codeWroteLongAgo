/*********************************************************************************
 *      Copyright:  (C) 2019 none
 *                  All rights reserved.
 *
 *       Filename:  analysis.c
 *    Description:  This file 
 *            
 *        Version:  1.0.0(03/30/2019)
 *         Author:  zhanghang <1711318490@qq.com>
 *      ChangeLog:  1, Release initial version on "03/30/2019 01:53:30 PM"
 *                 
 ********************************************************************************/
#include"analysis.h"

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
        return (short)val;
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
    else if(cmd==3)
    {
        val=buf[s];
        return (unsigned char)val;
    }
    return 0;
}
/* buf:the locaton of the data
 * len:the data's length(for byte)
 * dst:the container of the translated data
 */
void byte_to_string(unsigned char *buf,int len,unsigned char *dst)
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


int analysis_TLV_msg(unsigned char *buf,int rv,char *val_s)
{
    if(!buf||rv>MAXSIZE||!val_s)
    {
        return -1;
    }
    int i;
    unsigned char *ptr=NULL;
    unsigned short len;
    unsigned short crc_n;
    unsigned char tag;
    int           value;
    unsigned char value_s[1024];
    for(i=0;i<rv;i++)
    {
        if(buf[i]==0xFD&&i<rv-6)
        {
            ptr=&buf[i];
            tag=*(ptr+1);
            if(tag!=TLV_STR&&tag!=TLV_INT&&tag!=TLV_DOU&&tag!=TLV_CHR&&tag!=TLV_SHT)
                continue;
            len=*(ptr+2);
            if(len>rv-i)
                continue;
           // printf("%x   %x\n",buf[i+len-2],buf[i+len-1]);
            crc_n=crc_itu_t(MAGIC_CRC,ptr,len-2);//get crc from buf
           // printf("%x   %x\n",((unsigned char *)&crc_n)[1],((unsigned char *)&crc_n)[0]);
            if(((unsigned char *)&crc_n)[1]==buf[i+len-2]&&((unsigned char *)&crc_n)[0]==buf[i+len-1])
            {//compare the two crc bytes whether are all equal or not
                if(tag==TLV_STR)
                {
                    byte_to_string(ptr+3,len-5,value_s);
                    strncpy(val_s,value_s,MAXSIZE);
                }
                else if(tag==TLV_CHR)
                {
                    value=byte_to_int(ptr+3,3);
                    printf("%d\n",value);
                }
                else if(tag==TLV_SHT)
                {
                    value=byte_to_int(ptr+3,1);
                    printf("%d\n",value);
                }
                else if(tag==TLV_INT)
                {
                    value=byte_to_int(ptr+3,2);
                    printf("%d\n",value);
                }
                else if(tag==TLV_DOU)
                {
                    double a=byte_to_double(ptr+3);
                    printf("%f\n",a);
                }
            }
        }
    }
    return 0;
}
