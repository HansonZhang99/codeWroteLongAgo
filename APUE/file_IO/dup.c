#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 128

int main(int argc,char **argv)
{
    int fd1,fd2;
    char buf[MAX_SIZE]={0};
    const char *str="this is something writed to dup_fd\n";
    fd1=open("test_dup",O_CREAT|O_TRUNC|O_RDWR,0666);
    fd2=open("test_dup2",O_CREAT|O_TRUNC|O_RDWR,0666);
    if(fd1<0||fd2<0)
    {
        printf("open error\n");
        close(fd1);
        close(fd2);
        return -1;
    }
    int dup_fd=dup(fd1);
    close(fd1);
    dup2(fd2,STDOUT_FILENO);
    dup2(fd2,STDERR_FILENO);
    dup2(fd2,STDIN_FILENO);
    write(dup_fd,str,strlen(str)+1);
    int rv=read(dup_fd,buf,sizeof(buf));
    printf("before lseek, read %d bytes data from dup_fd:%s\n",rv,buf);
    lseek(dup_fd,0,SEEK_SET);
    rv=read(dup_fd,buf,sizeof(buf));
    printf("after lseek, read %d bytes data from dup_fd:%s\n",rv,buf);
    write(STDERR_FILENO,str,strlen(str)+1);
    printf("this is something printed to the screen\n");
    close(fd2);
    close(dup_fd);
    return 0;
}


