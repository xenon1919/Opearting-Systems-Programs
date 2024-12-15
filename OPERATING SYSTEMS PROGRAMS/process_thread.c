#include<stdio.h>
#include<pthread.h>
void* hi()
{
int a;
printf("\n I am a thread. \n");
printf("\n Enter a value.\n");
scanf("%d",&a);
printf("a= %d",a);
pthread_exit(NULL);
}
int main()
{
pthread_t tid;
printf("\n I am main process.\n");
pthread_create(&tid,NULL,hi,NULL);
pthread_join(tid,NULL); // Waiting for the created thread to terminate
printf("\nI came back to the main process.\n");
}
