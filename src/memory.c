/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/memory-private.h"
#include "../include/memory.h"


void *create_shared_memory(char *name, int size){
    return mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR), 0);
}

void *create_dynamic_memory(int size){
    return calloc(size, sizeof(int));
}

void destroy_shared_memory(char *name, void *ptr, int size){
    shm_unlink(name);
    munmap(ptr, size);
}

void destroy_dynamic_memory(void *ptr){
    free(ptr);
}

void write_rnd_access_buffer(struct rnd_access_buffer *buffer, int buffer_size, struct operation *op){
    for(int x = 0; x < buffer_size; x++){
      if (!buffer->ptr[x]){
            buffer->ptr[x] = 1;
            buffer->operacoes[x] = *op;
            return;
        }
    }
}

void write_circular_buffer(struct circular_buffer *buffer, int buffer_size, struct operation *op){
if (((buffer->input + 1) % buffer_size) != buffer->out){
        buffer->operacoes[buffer->input] = *op;
        buffer->input = (buffer->input + 1) % buffer_size;
    }
}

void read_rnd_access_buffer(struct rnd_access_buffer *buffer, int buffer_size, struct operation *op){
    for(int x = 0; x < buffer_size; x++){
      if (buffer->ptr[x]){
        buffer->ptr[x] = 0;
        *op = buffer->operacoes[x];
        return;
      }else{
        op->id = -1;
      }
    }
}

void read_circular_buffer(struct circular_buffer *buffer, int buffer_size, struct operation *op){
  if (buffer->input != buffer->out){
      *op = buffer->operacoes[buffer->out]; 
      buffer->out = (buffer->out + 1) % buffer_size;
    }else{
      op->id = -1;
    }
}