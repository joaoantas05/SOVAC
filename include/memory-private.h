/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#ifndef MEMORY_PRIVATE_H_GUARD
#define MEMORY_PRIVATE_H_GUARD
#include "memory.h"
#include "log.h"

/* estrutura que representa um buffer circular, completar pelos alunos */
struct circular_buffer{
  int out;
  int input;
  struct operation *operacoes;
};

/* estrutura que representa um buffer de acesso aleatório, completar pelos alunos */
struct rnd_access_buffer{
  int *ptr;
  struct operation *operacoes;
};

struct main_data *data;
struct communication_buffers *buffers;
struct semaphores *sems;
struct estrutura_logs *logs;
struct info_stats * stats;
struct info_alarme * alarmes;
int * alarme_tempo;

#endif