#ifndef  __OPT_INIT__
#define  __OPT_INIT__

#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "ctx.h"

int opt_init(char mqtt_username[],char mqtt_password[],char topic[],char broker_address[],int *broker_port,char server_ip[],int *server_port,\
                char ca_file[],char crt_file[],char key_file[],int argc,char **argv);
#endif
