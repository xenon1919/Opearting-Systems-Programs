#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
void main()
{
pid_t pid;
printf("\n hi, i am parent process my pid=%d and i am going to create a child\n",getpid());
pid=fork();
if(pid==0)
{
sleep(5);
printf("\n hi, i am child process my pid=%d and my parent pid=%d\n",getpid(),getppid());
}
else if(pid>0)
{
printf("\n hi, i am parent. i will wait for my child to complete\n");
wait(0); // Use sleep() to specify the waiting time and wait() to wait till the child completes its process
printf("\n i am waiting for my child to complete. Now i am exiting\n");
}
else
printf("\n error creating process\n");
}
