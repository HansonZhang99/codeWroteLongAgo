#ifndef  __OPT_INIT__
#define  __OPT_INIT__

#include<stdio.h>
#include<getopt.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include"analysis.h"

#define ARR_SIZE 128

int opt_init(int *port,char username[],char password[],char temp_topic[],char warn_topic[],char address[],int argc,char **argv);
#endif
