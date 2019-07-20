#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"string_break.h"
int string_break(char id[],char time[],char temper[],char buf[])
{
                memcpy(id,buf,strchr(buf,'/')-buf);
                *(strchr(buf,'/'))='\a';
                memcpy(time,strchr(buf,'\a')+1,strchr(buf,'/')-strchr(buf,'\a')-1);
                memcpy(temper,strchr(buf,'/')+1,strchr(buf,'\0')-strchr(buf,'/'));
		return 0;
}
