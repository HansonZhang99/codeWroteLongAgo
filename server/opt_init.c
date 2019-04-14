#include<stdio.h>
#include<getopt.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include"opt_init.h"
void usage(char *arg)
{
        printf("%s usage:\n",arg);
        puts("-p (--port)get the server port");
        puts("-h (--help)get the help message");
        return ;
}

int opt_init(int *port,int argc,char **argv)
{
        struct option opts[]=
        {
                {"port",required_argument,NULL,'p'},
		{"daemon",no_argument,NULL,'d'},
                {"help",no_argument,NULL,'h'},
                {NULL,0,NULL,0}
        };
        int       rv;
        while((rv=getopt_long(argc,argv,"dp:h",opts,NULL))!=-1)
        {
                switch(rv)
                {
			case 'd':
				if(daemon(0,0)<0)
				{
					printf("daemon error\n");
					return 0;
				}
                        case 'p':
                                *port=atoi(optarg);
                                break;
                        case 'h':
                                usage(argv[0]);
                                return 0;
			default:
				break;
                }
        }
        if(!*port)
        {
                usage(argv[0]);
                return -1;
        }
}




                                                                                                                        
