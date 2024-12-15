#include <stdio.h>
#include<sys/file.h>
#include<sys/stat.h>
#include <fcntl.h>
#include <err.h>
void main()
{
int fd;
fd=creat("file.txt",S_IRWXU|S_IRWXO|S_IRWXG);
if(fd==-1)
printf("file open error");
else
printf("fd=%d",fd);
}
