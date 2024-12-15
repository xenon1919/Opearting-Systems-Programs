#include<stdio.h>
#include<unistd.h>
int main()
{
pid_t pid;
printf("\n hi, i am parent process my pid=%d, i am going to create a child\n",getpid());
pid=fork();
printf("\n common area-1 now accessed by pid=%d \n",getpid());
if(pid==0)
printf("\n hi, i am child process my pid=%d my parents pid=%d\n",getpid(),getppid());
else if(pid>0)
printf("\n this is parent's private area\n");
else if(pid<0)
printf("\n error creating process\n");
else
printf("\n unknown error\n");
printf("\n common area-2 now accessed by pid=%d \n",getpid());
}
