#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h> // Added for sleep function

sem_t mutex;
sem_t db;
int readercount = 0;

void *reader(void *);
void *writer(void *);

int main() {
  sem_init(&mutex, 0, 1);
  sem_init(&db, 0, 1);

  pthread_t reader1, reader2, writer1, writer2;

  pthread_create(&reader1, NULL, reader, "1");
  pthread_create(&reader2, NULL, reader, "2");
  pthread_create(&writer1, NULL, writer, "1");
  pthread_create(&writer2, NULL, writer, "2");

  pthread_join(reader1, NULL);
  pthread_join(reader2, NULL);
  pthread_join(writer1, NULL);
  pthread_join(writer2, NULL);

  sem_destroy(&mutex);
  sem_destroy(&db);

  return 0;
}

void *reader(void *p) {
  printf("Previous value of mutex: %p\n", (void *)&mutex);

  sem_wait(&mutex);
  printf("Mutex acquired by reader: %p\n", (void *)&mutex);

  readercount++;
  if (readercount == 1)
    sem_wait(&db);

  sem_post(&mutex);
  printf("Mutex released by reader: %p\n", (void *)&mutex);

  printf("Reader %s is reading\n", (char *)p);

  // Simulating some reading time
  sleep(1);

  sem_wait(&mutex);
  printf("Reader %s completed reading\n", (char *)p);

  readercount--;
  if (readercount == 0)
    sem_post(&db);

  sem_post(&mutex);

  return NULL;
}

void *writer(void *p) {
  printf("Writer is waiting\n");

  sem_wait(&db);

  printf("Writer %s is writing\n", (char *)p);

  // Simulating some writing time
  sleep(2);

  sem_post(&db);

  return NULL;
}
