/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/synchronization.h"

void mostra_valores_semaforos (struct prodcons* sem){
    int full; sem_getvalue(sem->full, &full);
    int empty; sem_getvalue(sem->empty, &empty);
    int mutex; sem_getvalue(sem->mutex, &mutex);
    printf("full: %d ; empty: %d ; mutex: %d\n", full, empty,mutex);
}

sem_t *semaphore_create(char *name, int value){
  sem_t * semaforo = sem_open(name, O_CREAT, 0xFFFFFFFF, value);

  if (semaforo == SEM_FAILED){
    perror(name);
  }
  return semaforo;
}

void semaphore_destroy(char *name, sem_t *new_semaphore){
  if (sem_close(new_semaphore) == -1){
    perror(name);
  }

  if (sem_unlink(name) == -1){
    perror(name);
  }
}

void produce_begin(struct prodcons *pc){
  semaphore_mutex_lock(pc->empty);
  semaphore_mutex_lock(pc->mutex);
}

void produce_end(struct prodcons *pc){
  semaphore_mutex_unlock(pc->mutex);
  semaphore_mutex_unlock(pc->full);
}

void consume_begin(struct prodcons *pc){
  semaphore_mutex_lock(pc->full);
  semaphore_mutex_lock(pc->mutex);
}

void consume_end(struct prodcons *pc){
  semaphore_mutex_unlock(pc->mutex);
  semaphore_mutex_unlock(pc->empty);
}

void semaphore_mutex_lock(sem_t *sem){
  sem_wait(sem);
}

void semaphore_mutex_unlock(sem_t *sem){
  sem_post(sem);
}