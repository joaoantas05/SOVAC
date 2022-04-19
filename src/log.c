/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/memory-private.h"
#include "../include/memory.h"
#include "../include/log.h"
#include "../include/sotime.h"

void cria_log(struct estrutura_logs *logs){
  logs->nome_ficheiro = create_dynamic_memory(sizeof(char));
  logs->logs = create_dynamic_memory(sizeof(struct info_log));
  logs->ptr = 0;
}

void escreve_log(struct estrutura_logs *logs, char *option, int id){
  if(id == -1){
  logs->logs = (struct info_log *)realloc(logs->logs, (logs->ptr + 1) * sizeof(struct info_log));
  struct info_log novo_log;
  strcpy(novo_log.opcao, option);
  adicionar_tempo(&(novo_log.horas));
  logs->logs[logs->ptr] = novo_log;
  }else{
    logs->logs[logs->ptr].id = id;
    logs->ptr++;
  }
}

void escreve_ficheiro_log(struct estrutura_logs *logs){
  FILE * file_log = fopen(logs->nome_ficheiro, "w");
  for(int x = 0; x <= logs->ptr; x++){
    char tempo[22];
    adicionar_tempo_tostring(&logs->logs[x].horas, tempo);
    if(!strcmp(logs->logs[x].opcao, "read")){
      fprintf(file_log, "%s %s %d\n", tempo, logs->logs[x].opcao, logs->logs[x].id);
    }else{
      fprintf(file_log, "%s %s\n", tempo, logs->logs[x].opcao);
    }
    
  }
  fclose(file_log);
}

void destroi_log(struct estrutura_logs *logs){
  destroy_dynamic_memory(logs->nome_ficheiro);
  destroy_dynamic_memory(logs->logs);
  destroy_dynamic_memory(logs);
}