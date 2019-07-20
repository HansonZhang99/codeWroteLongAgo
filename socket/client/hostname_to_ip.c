#include<netdb.h>
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<arpa/inet.h>
#include"hostname_to_ip.h"
int hostname_to_ip(char * hostname,char array[16])
{
        struct hostent *host;
        const char  *ptr;
        char     buf[16];
        host=gethostbyname(hostname);
        if(host==NULL)
        {
                printf("Use function gethostbyname failure:%s\n",strerror(errno));
                return -1;
        }
        ptr=inet_ntop(host->h_addrtype,*(host->h_addr_list),buf,sizeof(buf));
        if(ptr==NULL)
        {
                printf("Use function inet_ntop failure:%s\n",strerror(errno));
                return -1;
        }
        strcpy(array,ptr);
        return 0;
}
