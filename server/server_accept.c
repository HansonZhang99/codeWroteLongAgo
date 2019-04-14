#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"server_accept.h"

int server_accept(int listen_fd)
{
	int 			accept_fd;
	struct sockaddr_in	client_addr;
	socklen_t               client_len;
	if((accept_fd=accept(listen_fd,(struct sockaddr*)&client_addr,&client_len))<0)
	{	
		printf("Accept failure:%s\n",strerror(errno));
//		close(listen_fd);
		return -1;
	}
	printf("Accept new client[%s:%d] successfully\n",inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
	return accept_fd;
}
	
