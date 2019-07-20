#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"server_socket_init.h"
int server_socket_init(int port,struct sockaddr_in *server_addr)
{
	int     listen_fd;
	int     bind_value;
	int     listen_value;
	int	on=1;
	listen_fd=socket(AF_INET,SOCK_STREAM,0);
	if(listen_fd<0)
	{
		printf("Using function socket failure:%s\n",strerror(errno));
		return -1;
	}
	setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	printf("Create new socket with fd[%d] successfully\n",listen_fd);
	memset(server_addr,9,sizeof(*server_addr));
	server_addr->sin_family=AF_INET;
	server_addr->sin_port=htons(port);
	server_addr->sin_addr.s_addr=htonl(INADDR_ANY);
	bind_value=bind(listen_fd,(struct sockaddr*)server_addr,sizeof(*server_addr));
	if(bind_value<0)
	{
		printf("Using function bind failure:%s\n",strerror(errno));
		close(listen_fd);
		return -1;
	}
	printf("Socket [%d] bind on port [%d] for all IP address successfully\n",listen_fd,port);
	listen_value=listen(listen_fd,10);
	if(listen_value<0)
	{
		printf("Using listen function failure:%s\n",strerror(errno));
		close(listen_fd);
		return -1;
	}
	printf("Socket start to listen port[%d] for all IP address\n",port);
	return listen_fd;
}
	
