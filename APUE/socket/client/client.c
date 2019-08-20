#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<syslog.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include"client_init.h"
#include"connect_init.h"
#include"get_temperature.h"
#include"get_time.h"
#include"hostname_to_ip.h"
#include"set_signal.h"
#include"opt_init.h"

//int g_stop=0;
int main(int argc,char **argv)
{
        int                port=0;//initialize for opt_init function
        char               name[128];
        struct sockaddr_in addr;
        int                conn_fd;
        char               *str="RPI0001";
        char               buf[128];
        char               datetime[32];//size
        float              temper;
        char               temper_s[10];
        int                rv=-1;
        int                rw=-1;
//        openlog("client",LOG_CONS|LOG_PID,0);
        if(set_signal()<0)
        {
                printf("set_signal() failure:%s\n",strerror(errno));
                return 0;
        }
        if(opt_init(&port,name,argc,argv)<0)
        {
                printf("opt_init failure:%s\n",strerror(errno));
                return 0;
        }
        if((conn_fd=client_init(port,name,&addr))<0)
        {
                printf("client_init failure:%s\n",strerror(errno));
                return 0;
        }
        while(!g_stop)
        {
		if(get_temperature(&temper)<0)
		{
			printf("get_temeprature error:%s\n",strerror(errno));
			return -1;
		}
		if(get_time(datetime)<0)
		{
			printf("get_time error:%s\n",strerror(errno));
			return -1;
		}
		snprintf(buf,128,"%s/%s/%f",str,datetime,temper);
                if(rv<0)
                {
                        printf("Try to connect to server\n");
                        if((rv=connect_init(&conn_fd,addr))<0)
                        {
                                printf("connect_init failure:%s\n");
                                sleep(10);
                                continue;
                        }
                }
                if((rw=write(rv,buf,sizeof(buf)))<0)
                {
                        printf("write failure:%s\n",strerror(errno));
                        close(rv);
                        rv=-1;
                }
                else
                {
                        printf("Write %d bytes data to server:%s\n",strlen(buf),buf);
                }
                sleep(30);
        }
}
                        


                                



        


