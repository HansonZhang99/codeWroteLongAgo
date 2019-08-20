#include <stdlib.h>//system
#include <unistd.h>//access
#include <stdio.h>//snprintf printf

#define FILE_NAME 64
int main()
{
    const char *name="test.txt";
    char buf[FILE_NAME];
    snprintf(buf,FILE_NAME,"touch %s",name);

    if(system(buf)==-1)
    {
        printf("system error\n");
        return -1;
    }
    if(access(name,F_OK))
    {
        printf("file %s does not exist\n",name);
        return -1;
    }
    if(!access(name,R_OK))
    {
        printf("file %s is readable\n",name);
    }
    if(!access(name,W_OK))
    {
        printf("file %s is writable\n",name);
    }
    if(!access(name,X_OK))
    {
        printf("file %s is excutable\n",name);
    }
    return 0;
}

