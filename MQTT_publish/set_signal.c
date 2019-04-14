#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include"set_signal.h"


int g_stop=0;
void signal_action(int signum)
{
        if(signum==SIGKILL)
        {
                g_stop=1;
                printf("kill signal makes program ended\n");
        }
        else if(signum==SIGINT)
        {
                g_stop=1;
                printf("Ctrl+C signal makes program ended\n");
        }
        exit(0);
}

int set_signal(void)
{
        struct sigaction        sigact;
        sigemptyset(&sigact.sa_mask);
        sigact.sa_flags=0;
        sigact.sa_handler=signal_action;

        sigaction(SIGKILL,&sigact,0);
        sigaction(SIGINT,&sigact,0);
        return 0;
}
