#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<pthread.h>
#include"string_break.h"
#include"insert_db.h"
void* thread_read(void *fd_s)
{
        int        *fd;
        fd=(int *)fd_s;
        int        rv;
	    float      temperature;
	    char       id[128];
	    char       temper[128];
	    char       time[128];
        char       buf[128];
        printf("New thread  create Ok,and now thread will communicate with clinet[%d]\n",*fd);
        while(1)
        {
		        memset(buf,0,sizeof(buf));
                if((rv=read(*fd,buf,sizeof(buf)))<0)
                {  
                        printf("Read message from client[%d] failure:%s\n",*fd,strerror(errno));
                        close(*fd);
                        pthread_exit(NULL);
                }
		
                else if(rv==0)
                {  
                        printf("The connection between client[%d] and server broke\n",*fd);
                        close(*fd);
                        pthread_exit(NULL);
                }
                else
                {
                        printf("Read %d bytes data from client[%d]:%s\n",strlen(buf),*fd,buf);
                }
		        string_break(id,time,temper,buf);
		        insert_db("messages","message",id,time,temper);
		        rv=-1;        
}
	
	
        close(*fd);
}   
    

