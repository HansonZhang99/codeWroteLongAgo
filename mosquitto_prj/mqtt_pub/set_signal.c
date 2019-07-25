#include"set_signal.h"

void signal_action(int signum)
{
    if(signum==SIGINT)
    {
        int fd=open(BUZZER_PATH,O_RDWR);
        if(fd<0)
        {
            printf("open %s error\n",BUZZER_PATH);
            exit(0) ;
        }
        ioctl(fd,OFF,0);
        int e=system("echo 0 > /sys/class/leds/led1/brightness");
        int f=system("echo 0 > /sys/class/leds/led2/brightness");
        int g=system("echo 0 > /sys/class/leds/led3/brightness");
        int h=system("echo 0 > /sys/class/leds/led4/brightness");
        if(e||f||g||h)
        {
            printf("system error\n");
            exit(0);
        }
    }
    exit(0);
}

int set_signal(void)
{
    struct sigaction        sigact;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags=0;
    sigact.sa_handler=signal_action;

    sigaction(SIGINT,&sigact,0);
    return 0;
}
