#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include"client_init.h"
#include"hostname_to_ip.h"
int client_init(int port ,char *name,struct sockaddr_in *server_addr)
{
        int                   conn_fd;
      //  struct sockaddr_in    server_addr;
        char                  array[16];
        conn_fd=socket(AF_INET,SOCK_STREAM,0);
        if(conn_fd<0)
        {
                printf("Using function socket failure:%s\n",strerror(errno));
                return -1;
        }
        memset(server_addr,0,sizeof(*server_addr));
/*
        for(i=0;i<argc;i++)
        {
                if(strcmp("-n",argv[i])==0)
                {
                        hostnametoip(name,array[16]);
                        inet_aton(array,&server_addr.sin_addr);
                        break;
                }
                else if(strcmp("-i",argv[i])==0)
                {
                        inte_aton(name,&server_addr.sin_addr);
                        break;
                }
        }
*/
        if(name[0]>'9')
        {
                hostname_to_ip(name,array);
        //        printf("%s\n",array);
                inet_aton(array,&server_addr->sin_addr);
        }
        else
        {
                inet_aton(name,&server_addr->sin_addr);
        }
        server_addr->sin_family=AF_INET;
        server_addr->sin_port=htons(port);
        //addr=server_addr;
        printf("initialize client successfully\n");
        return conn_fd;
}
