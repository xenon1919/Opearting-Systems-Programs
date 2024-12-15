#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<err.h>
void main()
{
int fd,n;
char inf[]="First Program";
fd=open("file.txt",O_RDWR);
if(fd==-1)
printf("file not opened\n");
else
printf("file opened with fd=%d\n",fd);
//write into file
n= write(fd,inf,sizeof(inf));
if(n==-1)
{
printf("write operation unsuccessful\n");
}
else{
printf("write operation successful\n");
}
close(fd);
}
