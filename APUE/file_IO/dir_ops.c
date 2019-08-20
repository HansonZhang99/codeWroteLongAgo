#include <sys/stat.h>//mkdir creat
#include <sys/types.h>//mkdir opendir creat
#include <dirent.h>//readdir opendir
#include <unistd.h>//chdir
#include <fcntl.h>//creat
#include <stdio.h>
int main()
{
    DIR*   dirp;
    struct dirent *direntp;
    const char *dir_name="dir";
    const char *file_name1="test.txt";
    const char *file_name2="file.txt";
    if(mkdir(dir_name,0755))
    {
        printf("mkdir error\n");
        return -1;
    }
    if(chdir(dir_name))
    {
        printf("chdir error\n");
        return -1;
    }
    if(creat(file_name1,0644)<0)
    {
        printf("create file %s error\n",file_name1);
        return 1;
    }
    if(creat(file_name2,0644)<0)
    {
        printf("create file %s error\n",file_name2);
        return -1;
    }
    if(mkdir(dir_name,0755))
    {
        printf("mkdir error\n");
        return -1;
    }
    if(chdir("../"))
    {
        printf("chdir error\n");
        return -1;
    }
    if((dirp=opendir(dir_name))==NULL)
    {
        printf("opendir error\n");
        return -1;
    }
    while((direntp=readdir(dirp))!=NULL)
    {
        printf("%s\n",direntp->d_name);
    }
    return 0;
} 

