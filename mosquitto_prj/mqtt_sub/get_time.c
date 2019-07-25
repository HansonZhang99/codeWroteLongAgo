#include"get_time.h"
int get_time(char time_s[24])
{
        time_t   timec;
        char *str;
        time(&timec);
        str=ctime(&timec);
        char *chr=strchr(str,'\n');
        *chr='\0';
        strcpy(time_s,str);
        return 0;

}

