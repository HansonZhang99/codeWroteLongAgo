#include"opt_init.h"

void usage(char *arg)
{
        printf("%s usage:\n",arg);
        puts("-u (--mqttusername)the mqtt username(required_argument)");
        puts("-p (--mqttpassword)the mqtt password(required_argument)");
        puts("-t (--temptopic)the publish topic(required_argument)");
        puts("-b (--brokeraddress)the mqtt broker address(required_argument)");
        puts("-o (--brokerport)the mqtt broker port(required_argument)");
        puts("-i (--serverip)the listen ip address(required_argument)");
        puts("-s (--serverport)the listen port(required_argument)");
        puts("-c (--cafilepath)the CA certificate path(required_argument)");
        puts("-r (--crtfilepath)the server certificate path(required_argument)");
        puts("-k (--keyfilepath)the server key path(required_argument)");
	    puts("-d (--daemon)run back");
        puts("-h (--help)get the help message");
        return ;
}

int opt_init(char mqtt_username[],char mqtt_password[],char topic[],char broker_address[],int *broker_port,char server_ip[],int *server_port,\
        char ca_file[],char crt_file[],char key_file[],int argc,char **argv)
{
    if(!mqtt_username||!mqtt_password||!topic||!broker_address||!broker_port||!server_ip||!server_port||!ca_file||!crt_file||!key_file)
        return -1;
    struct option opts[]=
    {
        {"mqttusername",required_argument,NULL,'u'},
        {"mqttpassword",required_argument,NULL,'p'},
        {"temptopic",required_argument,NULL,'t'},
        {"brokeraddress",required_argument,NULL,'b'},
        {"brokerport",required_argument,NULL,'o'},
        {"serverip",required_argument,NULL,'i'},
        {"serverport",required_argument,NULL,'s'},
        {"cafilepath",required_argument,NULL,'c'},
        {"crtfilepath",required_argument,NULL,'r'},
        {"keyfilepath",required_argument,NULL,'k'},
        {"daemon",no_argument,NULL,'d'},
        {"help",no_argument,NULL,'h'},
        {NULL,0,NULL,0}
    };
    int       rv;
    int       sum=0;
    while((rv=getopt_long(argc,argv,"u:p:t:b:o:i:s:c:r:k:dh",opts,NULL))!=-1)
    {
        switch(rv)
        {
            case 'u':
                strncpy(mqtt_username,optarg,ARR_SIZE-1);
                sum++;
                break;
            case 'p':
                strncpy(mqtt_password,optarg,ARR_SIZE-1);
                sum++;
                break;
            case 't':
                strncpy(topic,optarg,ARR_SIZE-1);
                sum++;
                break;
            case 'b':
                strncpy(broker_address,optarg,ARR_SIZE-1);
                sum++;
                break;
            case 'o':
                *broker_port=atoi(optarg);
                sum++;
                break;
            case 'i':
                strncpy(server_ip,optarg,ARR_SIZE-1);
                sum++;
                break;
            case 's':
                *server_port=atoi(optarg);
                sum++;
                break;
            case 'c':
                strncpy(ca_file,optarg,ARR_SIZE-1);
                sum++;
                break;
            case 'r':
                strncpy(crt_file,optarg,ARR_SIZE-1);
                sum++;
                break;
            case 'k':
                strncpy(key_file,optarg,ARR_SIZE-1);
                sum++;
                break;
            case 'd':
                if(daemon(0,0)<0)
                {
                    printf("daemon error:[%d] [%s]\n",errno,strerror(errno));
                    return -1;
                }
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                usage(argv[0]);
                return -1;
        }
    }
    if(sum!=10)
    {
        usage(argv[0]);
        return -1;
    }
    return 0;
}

                                                                                                                        
