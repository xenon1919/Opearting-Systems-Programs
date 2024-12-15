#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXTHREAD 4 /* define the number of readers */

int access_database(int);
void write_database(); /* Forward declaration */

void *reader(void *);
void *writer(void *);

sem_t mutex; /* controls access to rc */
sem_t db;    /* controls access to db */

int rc = 0; /* number of processes reading or wanting to */
int dbase[MAXTHREAD];

int running = 1; /* Flag to control program termination */

void handle_signal(int signal) {
  if (signal == SIGINT) {
    printf("Terminating the program...\n");
    running = 0; /* Set the flag to stop the program */
  }
}

int main() {
  pthread_t readers[MAXTHREAD], writerTh;
  int index;
  int ids[MAXTHREAD];

  sem_init(&mutex, 0, 1);
  sem_init(&db, 0, 1);

  signal(SIGINT, handle_signal); /* Register signal handler for SIGINT */

  for (index = 0; index < MAXTHREAD; index++) {
    ids[index] = index + 1;

    if (pthread_create(&readers[index], NULL, reader, &ids[index]) != 0) {
      perror("Cannot create reader!");
      exit(1);
    }
  }

  if (pthread_create(&writerTh, NULL, writer, NULL) != 0) {
    perror("Cannot create writer");
    exit(1);
  }

  sleep(10); /* Run the program for 10 seconds */

  running = 0; /* Set the flag to stop the program */

  pthread_join(writerTh, NULL);

  sem_destroy(&mutex);
  sem_destroy(&db);

  return 0;
}

void *reader(void *arg) {
  int index = *(int *)arg;
  int value;

  while (running) {
    sem_wait(&mutex);
    rc++;
    if (rc == 1)
      sem_wait(&db);
    sem_post(&mutex);

    value = access_database(index - 1);

    printf("Thread %d read %d\n", index, value);
    sleep(index);

    sem_wait(&mutex);
    rc--;
    if (rc == 0)
      sem_post(&db);
    sem_post(&mutex);
  }

  return NULL;
}

void *writer(void *arg) {
  while (running) {
    sem_wait(&db);
    write_database();
    printf("Writer is now writing... Number of readers: %d\n", rc);
    sleep(1);
    sem_post(&db);
  }

  return NULL;
}

int access_database(int index) { return dbase[index]; }

void write_database() {
  int i;
  for (i = 0; i < MAXTHREAD; i++) {
    dbase[i] = rand() % 1000;
    // printf("...writing: dbase[%d] = %d\n", i, dbase[i]);
  }
}
