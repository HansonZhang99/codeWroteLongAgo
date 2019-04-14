#include<stdio.h>
#include<getopt.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include"opt_init.h"
void usage(char *arg)
{
        printf("%s usage:\n",arg);
        puts("-p (--port)get the port");
	puts("-d (--daemon)run back");
	puts("-a (-address)the subscribe address");
        puts("-h (--help)get the help message");
	puts("-i (--id) get the subscribe id");
	puts("-t (--topic)the subscribe topic");
        return ;
}

int opt_init(int *port,char name[],char sub_id[],char topic[],int argc,char **argv)
{
        struct option opts[]=
        {
                {"port",required_argument,NULL,'p'},
		{"daemon",no_argument,NULL,'d'},
		{"topic",required_argument,NULL,'t'},
		{"sub_id",required_argument,NULL,'i'},
		{"address",required_argument,NULL,'a'},
                {"help",no_argument,NULL,'h'},
                {NULL,0,NULL,0}
        };
        int       rv;
        while((rv=getopt_long(argc,argv,"d:p:a:i:t:h",opts,NULL))!=-1)
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
			case 'a':
				strcpy(name,optarg);
				break;
			case 'i':
				strcpy(sub_id,optarg);
				break;
			case 't':
				strcpy(topic,optarg);
				break;
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
}




                                                                                                                        
