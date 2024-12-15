#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<err.h>
void main()
{
int fd,n;
char buf[50];
fd=open("file.txt",O_RDWR);
if(fd==-1)
printf("file not opened\n");
else
printf("file opened with fd=%d\n",fd);
//read from file
n=read(fd,buf,sizeof(buf));
printf("The data in file is: %s\n",buf);
close(fd);
}
