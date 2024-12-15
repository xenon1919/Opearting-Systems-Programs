#include <stdio.h>
#include <unistd.h>
int main() {
  pid_t pid;
  printf("hi, i am parent process my pid=%d, i am going to create a child\n",
         getpid());
  pid = fork();
  printf("common area -1 now accessed by pid=%d\n", getpid());
  if (pid == 0) {
    printf("hi, i am child process my pid=%d my parent pid=%d", getpid(),
           getppid());
  } else if (pid > 0) {
    printf("this is parent private area\n");
  } else if (pid < 0) {
    printf("error in creating process\n");
  } else {
    printf("unknown error");
  }
  printf("\n common area -2 now accessed by pid=%d\n", getpid());
}