#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"connect_init.h"
int connect_init(int *conn_fd,struct sockaddr_in addr)
{
        int    conn_value;
        conn_value=connect(*conn_fd,(struct sockaddr*)&addr,sizeof(addr));
        if(conn_value<0)
        {
                printf("Using function connect  failure:%s\n",strerror(errno));
                return -1;
        }
        printf("client connect to server [%s:%d] successfully\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
        return *conn_fd;
        
}



