/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#ifndef LOG_H_GUARD
#define LOG_H_GUARD

#include <time.h>

//estrutura que guarda as informações de um log
struct info_log{
  char opcao[100];
  int id;
  struct timespec horas;
};

//estrutura que guarda as informações de todos os logs
struct estrutura_logs{
  struct info_log *logs;
  char * nome_ficheiro;
  int ptr;
};

//Aloca memoria dinamica a um log
void cria_log(struct estrutura_logs *logs);

//Guarda os logs na estrutura
void escreve_log(struct estrutura_logs *logs, char *option, int id);

//Escreve as informações da estrutura dos logs num ficheiro
void escreve_ficheiro_log(struct estrutura_logs *logs);

//Remoce a memoria dinamica de um log
void destroi_log(struct estrutura_logs *logs);

#endif