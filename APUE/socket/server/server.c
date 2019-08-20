#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include"db_init.h"
#include"insert_db.h"
#include"opt_init.h"
#include"server_socket_init.h"
#include"server_accept.h"
#include"thread_init.h"
#include"thread_read.h"
#include"set_signal.h"

int main(int argc,char **argv)
{
	int          		port;
	pthread_t               tid;
	int                     listen_fd,accept_fd;
	struct sockaddr_in	server_addr;
    if(set_signal()<0)
    {
            printf("set_signal failure:%s\n",strerror(errno));
            return 0;
    }
	if(opt_init(&port,argc,argv)<0)
	{
		printf("opt_init failure:%s\n",strerror(errno));
		return -1;
	}
	if((listen_fd=server_socket_init(port,&server_addr))<0)
	{
		printf("server_socket_init failure:%s\n",strerror(errno));
		return -1;
	}
	if(db_init("messages","message")<0)
	{
		printf("db_init failure:%s\n",strerror(errno));
		return -1;
	}
	while(!g_stop)
	{
		if((accept_fd=server_accept(listen_fd))<0)
		{
			printf("server_accept failure:%s\n",strerror(errno));
			continue;
		}
		if(thread_init(tid,thread_read,&accept_fd)<0)
		{
			printf("thread_init failure:%s\n",strerror(errno));
		  	continue;
		}
	}
	close(listen_fd);
	return 0;
}	
	
