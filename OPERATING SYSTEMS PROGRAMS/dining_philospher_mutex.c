#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
// Buffer data structures
#define MAX_BUFFERS 10
char buf [MAX_BUFFERS] [100];
int buffer_index;
int buffer_print_index;
pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t spool_cond = PTHREAD_COND_INITIALIZER;
int buffers_available = MAX_BUFFERS;
int lines_to_print = 0;
pthread_mutex_t fork_mutex [5] = {PTHREAD_MUTEX_INITIALIZER, 
PTHREAD_MUTEX_INITIALIZER, 
 PTHREAD_MUTEX_INITIALIZER, 
PTHREAD_MUTEX_INITIALIZER, 
 PTHREAD_MUTEX_INITIALIZER};
void *philosopher (void *arg);
void *spooler (void *arg);
void print_string (const char *const str);
int main (int argc, char **argv)
{
 pthread_t tid_philosopher [5], tid_spooler;
 int i, r;
 // initialization
 buffer_index = buffer_print_index = 0;
 time_t now = time (NULL);
 srand ((unsigned int) (now % 937));
 // Create spooler
 if ((r = pthread_create (&tid_spooler, NULL, spooler, NULL)) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
 }
 // Create 5 philosopher threads
 int thread_no [5];
 for (i = 0; i < 5; i++) {
 thread_no [i] = i;
 if ((r = pthread_create (&tid_philosopher [i], NULL, philosopher, 
(void *) &thread_no [i])) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
 }
 }
 // Wait for philosophers to terminate
 for (i = 0; i < 5; i++)
 if ((r = pthread_join (tid_philosopher [i], NULL)) == -1) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
 }
 
 // No more strings to print?
 while (lines_to_print) sleep (1);
 // terminate spooler
 if ((r = pthread_cancel (tid_spooler)) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
 }
 exit (0);
}
/* philosopher: think and eat
 There are five philosopher threads
 Philosopher 0 uses forks 0 and 1
 Philosopher 1 uses forks 1 and 2
 Philosopher 2 uses forks 2 and 3
 Philosopher 3 uses forks 3 and 4
 Philosopher 4 uses forks 0 and 4
*/
void *philosopher (void *arg)
{
 int i, r, fork1, fork2;
 int my_id = *((int *) arg);
 char my_str [100];
 while (1) {
 // think or eat?
 bool eat = rand () % 2;
if (eat) {
 sprintf (my_str, "P %d: %ld Will eat!\n", my_id, time (NULL));
 print_string (my_str);
 // get first fork
 fork1 = (my_id == 4) ? 0 : my_id;
 if ((r = pthread_mutex_lock (&fork_mutex [fork1])) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit 
(1);
 }
 // get second fork
 fork2 = (my_id == 4) ? 4 : my_id + 1;
 if ((r = pthread_mutex_lock (&fork_mutex [fork2])) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit 
(1);
 }
 sprintf (my_str, "P %d: %ld Got forks: %d and %d\n", my_id, time 
(NULL), fork1, fork2);
 print_string (my_str);
 sprintf (my_str, "P %d: %ld Eating....\n", my_id, time (NULL));
 print_string (my_str);
 sleep (rand () % 5 + 1);
 // release first fork
 if ((r = pthread_mutex_unlock (&fork_mutex [fork1])) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit 
(1);
 }
 // release second fork
 if ((r = pthread_mutex_unlock (&fork_mutex [fork2])) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit 
(1);
 }
}
else { //think
 sprintf (my_str, "P %d: %ld Thinking....\n", my_id, time (NULL));
 print_string (my_str);
 sleep (rand () % 5 + 1);
}
 sprintf (my_str, "P %d: %ld Finished %s", my_id, time (NULL), (eat) ? 
"eating.\n" : "thinking.\n");
 print_string (my_str);
 }
}
// print a string on the terminal
void print_string (const char *const str)
{
 int r;
 // Lock mutex
 if ((r = pthread_mutex_lock (&buf_mutex)) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
 }
 while (!buffers_available) 
 pthread_cond_wait (&buf_cond, &buf_mutex);
 int j = buffer_index;
 buffer_index++;
 if (buffer_index == MAX_BUFFERS)
 buffer_index = 0;
 buffers_available--;
strcpy (buf [j], str);
 lines_to_print++;
 pthread_cond_signal (&spool_cond);
 // Unlock mutex
 if ((r = pthread_mutex_unlock (&buf_mutex)) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
 }
 
}
// There is only one spooler thread
void *spooler (void *arg)
{
 int r;
 while (1) { // forever
 // Lock mutex
 if ((r = pthread_mutex_lock (&buf_mutex)) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
 }
 while (!lines_to_print) 
 pthread_cond_wait (&spool_cond, &buf_mutex);
 printf ("%s", buf [buffer_print_index]);
 lines_to_print--;
 buffer_print_index++;
 if (buffer_print_index == MAX_BUFFERS)
 buffer_print_index = 0;
 buffers_available++;
 pthread_cond_signal (&buf_cond);
 // Unlock mutex
 if ((r = pthread_mutex_unlock (&buf_mutex)) != 0) {
 fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
 }
 }
}
