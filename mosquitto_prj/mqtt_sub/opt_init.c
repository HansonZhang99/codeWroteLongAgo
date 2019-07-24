#include"opt_init.h"


void usage(char *arg)
{
        printf("%s usage:\n",arg);
        puts("-p (--port)get the brokerport (required_argument)");
	    puts("-d (--daemon)run back");
	    puts("-a (-address)the broker address (required_argument)");
        puts("-h (--help)get the help message");
	    puts("-u (--username)the connect username (required_argument)");
	    puts("-t (--temptopic)the temperature topic (required_argument)");
        puts("-w (--warntopic)the warning topic (required_argument)");
        puts("-P (--Passwd)the connect password (required_argument)");
        return ;
}

int opt_init(int *port,char username[],char password[],char temp_topic[],char warn_topic[],char address[],int argc,char **argv)
{
    if(!port||!username||!password||!temp_topic||!warn_topic||!address)
        return -1;
    struct option opts[]=
    {
        {"port",required_argument,NULL,'p'},
        {"daemon",no_argument,NULL,'d'},
        {"temptopic",required_argument,NULL,'t'},
        {"warntopic",required_argument,NULL,'w'},
        {"username",required_argument,NULL,'u'},
        {"address",required_argument,NULL,'a'},
        {"Passwd",required_argument,NULL,'P'},
        {"help",no_argument,NULL,'h'},
        {NULL,0,NULL,0}
    };
    int       rv;
    while((rv=getopt_long(argc,argv,"p:da:hu:t:w:P:",opts,NULL))!=-1)
    {
        switch(rv)
        {
            case 'd':
                if(daemon(0,0)<0)
                {
                    printf("daemon error\n");
                    return -1;
                }
            case 'p':
                *port=atoi(optarg);
                break;
            case 'a':
                strncpy(address,optarg,ARR_SIZE);
                break;
            case 'u':
                strncpy(username,optarg,ARR_SIZE);
                break;
            case 'P':
                strncpy(password,optarg,ARR_SIZE);
                break;
            case 't':
                strncpy(temp_topic,optarg,ARR_SIZE);
                break;
            case 'w':
                strncpy(warn_topic,optarg,ARR_SIZE);
                break;
            case 'h':
                usage(argv[0]);
                return -1;
            default:
                usage(argv[0]);
                return -1;
        }
    }
    if(port<=0||strlen(username)==0||strlen(password)==0||strlen(temp_topic)==0||strlen(warn_topic)==0||strlen(address)==0)
    {
        usage(argv[0]);
        return -1;
    }
    return 0;
}




                                                                                                                        
