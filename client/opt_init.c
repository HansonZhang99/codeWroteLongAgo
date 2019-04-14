#include<stdio.h>
#include<getopt.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include"opt_init.h"
void usage(char *arg)
{
        printf("%s usage:\n",arg);
        puts("-p (--port)get the server port");
        puts("-i (--ipaddress)get the server ip");
        puts("-n (--hostname)get the server hostname");
        puts("-d (--daemon)Program will run int the background");
        puts("-h (--help)get the help message");
        return ;
}

int opt_init(int *port,char name[],int argc,char **argv)
{
        struct option opts[]=
        {
                {"port",required_argument,NULL,'p'},
                {"ipaddress",required_argument,NULL,'i'},
                {"hostname",required_argument,NULL,'n'},
                {"daemon",no_argument,NULL,'d'},
                {"help",no_argument,NULL,'h'},
                {NULL,0,NULL,0}
        };
        int       rv;
        while((rv=getopt_long(argc,argv,"dp:i:n:h",opts,NULL))!=-1)
        {
                switch(rv)
                {
                        case 'i':
                                //name=optarg;
                                strcpy(name,optarg);
                                break;
                        case 'n':
                                //name=optarg;
                                strcpy(name,optarg);
                                break;
                        case 'p':
                                *port=atoi(optarg);
                                break;
                        case 'd':
                                if(daemon(0,0)<0)
                                {
                                        printf("daemon error\n");
                                        return 0;
                                }
                        case 'h':
                                usage(argv[0]);
                                return 0;
                        default:
                                break;
                }
        }
        if(!*port||!name)
        {
                usage(argv[0]);
                return -1;
        }
        return 0;
}



        
