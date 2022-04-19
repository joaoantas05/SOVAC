/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "../include/main.h"
#include "../include/configuration.h"
#include "../include/process.h"
#include "../include/log.h"
#include "../include/synchronization.h"
#include "../include/stats.h"
#include "../include/soalarm.h"
#include "../include/sosignal.h"
#include "../include/sotime.h"



void main_args(int argc, char *argv[], struct main_data *data){
  lerFicheiro(argc, argv, data, logs, stats);
}

void create_dynamic_memory_buffers(struct main_data *data){
  data->client_pids = create_dynamic_memory(data->n_clients * sizeof(int));
  data->client_stats = create_dynamic_memory(data->n_clients * sizeof(int));

  data->proxy_pids = create_dynamic_memory(data->n_proxies * sizeof(int));
  data->proxy_stats = create_dynamic_memory(data->n_proxies * sizeof(int));

  data->server_pids = create_dynamic_memory(data->n_servers * sizeof(int));
  data->server_stats = create_dynamic_memory(data->n_servers * sizeof(int));
}

void create_shared_memory_buffers(struct main_data *data, struct communication_buffers *buffers){
  buffers->main_cli = create_shared_memory(STR_SHM_MAIN_CLI_BUFFER, sizeof(struct rnd_access_buffer));
  buffers->main_cli->operacoes = create_shared_memory(STR_SHM_MAIN_CLI_BUFFER,  sizeof(struct operation) * data->max_ops);
  buffers->main_cli->ptr = create_shared_memory(STR_SHM_MAIN_CLI_PTR,sizeof(int) * data->max_ops);

  buffers->cli_prx = create_shared_memory(STR_SHM_CLI_PRX_BUFFER, sizeof(struct circular_buffer));
  buffers->cli_prx->operacoes = create_shared_memory(STR_SHM_CLI_PRX_BUFFER, sizeof(struct operation) * data->max_ops);

  buffers->prx_srv = create_shared_memory(STR_SHM_PRX_SRV_BUFFER, sizeof(struct rnd_access_buffer));
  buffers->prx_srv->operacoes = create_shared_memory(STR_SHM_PRX_SRV_BUFFER, sizeof(struct operation) * data->max_ops);

  buffers->prx_srv->ptr = create_shared_memory(STR_SHM_SRV_CLI_PTR, sizeof(int) * data->max_ops);
  buffers->srv_cli = create_shared_memory(STR_SHM_SRV_CLI_BUFFER, sizeof(struct circular_buffer));
  buffers->srv_cli->operacoes = create_shared_memory(STR_SHM_SRV_CLI_BUFFER, sizeof(struct operation) * data->max_ops);

  data->results = create_shared_memory(STR_SHM_RESULTS, sizeof(struct operation) * data->max_ops);
  data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
}

void create_semaphores(struct main_data *data, struct semaphores *sems){
  sems->main_cli = create_dynamic_memory(sizeof(struct prodcons));
  sems->cli_prx = create_dynamic_memory(sizeof(struct prodcons));
  sems->prx_srv = create_dynamic_memory(sizeof(struct prodcons));
  sems->srv_cli = create_dynamic_memory(sizeof(struct prodcons));

  sems->main_cli->empty = semaphore_create(STR_SEM_MAIN_CLI_EMPTY, data->buffers_size);
  sems->cli_prx->empty = semaphore_create(STR_SEM_CLI_PRX_EMPTY, data->buffers_size);
  sems->prx_srv->empty = semaphore_create(STR_SEM_PRX_SRV_EMPTY, data->buffers_size);
  sems->srv_cli->empty = semaphore_create(STR_SEM_SRV_CLI_EMPTY, data->buffers_size);

  sems->main_cli->full = semaphore_create(STR_SEM_MAIN_CLI_FULL, 0);
  sems->cli_prx->full = semaphore_create(STR_SEM_CLI_PRX_FULL, 0);
  sems->prx_srv->full = semaphore_create(STR_SEM_PRX_SRV_FULL, 0);
  sems->srv_cli->full = semaphore_create(STR_SEM_SRV_CLI_FULL, 0);

  sems->main_cli->mutex = semaphore_create(STR_SEM_MAIN_CLI_MUTEX, 1);
  sems->cli_prx->mutex = semaphore_create(STR_SEM_CLI_PRX_MUTEX, 1);
  sems->prx_srv->mutex = semaphore_create(STR_SEM_PRX_SRV_MUTEX, 1);
  sems->srv_cli->mutex = semaphore_create(STR_SEM_SRV_CLI_MUTEX, 1);
  
  sems->results_mutex = semaphore_create(STR_SEM_RESULTS_MUTEX, 1);

  puts("\n|****  Semáforos criados  ****|");
  mostra_valores_semaforos( sems -> main_cli);
  mostra_valores_semaforos( sems -> cli_prx);
  mostra_valores_semaforos( sems -> prx_srv);
  mostra_valores_semaforos( sems -> srv_cli);
  puts("|*****************************|");
}

void launch_processes(struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
  
  for (int x = 0; x < data -> n_clients; x++) {
    data -> client_pids[x] = launch_process(x, 0, buffers, data, sems);
  }

  for (int x = 0; x < data -> n_proxies; x++) {
    data -> proxy_pids[x] = launch_process(x, 1, buffers, data, sems);
  }
  
  for (int x = 0; x < data -> n_servers; x++) {
    data -> server_pids[x] = launch_process(x, 2, buffers, data, sems);
  }
}

void user_interaction(struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
  char opcao[5];
  int counter = 0;
  int acabou = 1;
  signal(SIGINT, signal_handling);
  signal(SIGALRM,signal_handling);
  alarm(3);
  while (acabou){
      puts("\n-> Introduza uma operação:");
      if(scanf(" %s", opcao) != 1) return;
      escreve_log(logs, opcao,-1);
      if (!strcmp(opcao, "op")){
        logs->ptr++;
        create_request(&counter, buffers, data, sems);
      }else if (!strcmp(opcao, "stop")){
        acabou--;
      }else if (!strcmp(opcao, "read")){
        read_answer(data, sems);
      }else if (!strcmp(opcao, "help")){
        puts("op - cria uma nova operação, através da função create_request");
        puts("read - verifica o estado de uma operação através da função read_answer");
        puts("stop - termina o execução do socps através da função stop_execution");
        logs->ptr++;
      }
  }

  stop_execution(data, buffers, sems);
}

void create_request(int *op_counter, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
  if (*op_counter <= data -> max_ops) {
    struct operation operacao;
    adicionar_tempo(&(operacao.start_time));
    operacao.id = *op_counter;
    printf("Operação %d criada.\n", operacao.id);
    produce_begin(sems->main_cli);
    write_rnd_access_buffer(buffers -> main_cli, data -> buffers_size, &operacao);
    produce_end(sems->main_cli);
    *op_counter += 1;
  } else {
    puts("Não é possivel criar mais operações.");
  }

}

void read_answer(struct main_data *data, struct semaphores *sems){
  int input;
  if(scanf("%d", &input) != 1) return;
  escreve_log(logs, "",input);
  if(input < data->max_ops && data->results[input].status == 'S'){
    printf("\nInformações sobre operação com o id %d:\n", input);
    printf("Status: %c\n", data -> results[input].status);
    printf("Recebido pelo cliente: %d\n", data -> results[input].client);
    printf("Encaminhado pelo proxy: %d\n", data -> results[input].proxy);
    printf("Servido pelo server: %d\n\n", data -> results[input].server);
  }else if(input >= data->max_ops){
    printf("O id %d é inválido.\n", input);
  }else{
    printf("A operação com o id %d não está disponivel.\n", input);
  }
}

void stop_execution(struct main_data *data, struct communication_buffers *buffers, struct semaphores *sems){
  *(data->terminate) = 1;
  wakeup_processes(data, sems);
  wait_processes(data);

  write_statistics(data);
  escreve_ficheiro_log(logs);
  stats_cria_ficheiro(logs, data, stats);

  destroi_log(logs);
  destroy_shared_memory_buffers(data, buffers);
  destroy_dynamic_memory_buffers(data);
  destroy_semaphores(sems);
  destroy_dynamic_memory(data);
  destroy_dynamic_memory(buffers);
  exit(0);
}

void wait_processes(struct main_data *data){
  for (int x = 0; x < data->n_clients; x++) { 
      data->client_stats[x]  += wait_process(data->client_pids[x]); 
    }
    for (int x = 0; x < data->n_proxies; x++) { 
      data->proxy_stats[x]   += wait_process(data->proxy_pids[x] ); 
    }
    for (int x = 0; x < data->n_servers; x++) { 
      data->server_stats[x]  += wait_process(data->server_pids[x]); 
    }
}

void write_statistics(struct main_data *data){
    puts("\n\n|***   Estatisticas   ***|");
  for (int x = 0; x < data -> n_clients; x++) {
    printf("Cliente: %d -> Pedidos: %d\n", x, data -> client_stats[x]);
  }
  for (int x = 0; x < data->n_proxies; x++) {
    printf("Servidor: %d -> Pedidos: %d\n", x, data -> server_stats[x]);
  }
  for (int x = 0; x < data->n_servers; x++) {
    printf("Proxy: %d -> Pedidos: %d\n", x, data -> proxy_stats[x]);
  }
  puts("|************************|");
  
}

void destroy_dynamic_memory_buffers(struct main_data *data){
  destroy_dynamic_memory(data->client_pids);
  destroy_dynamic_memory(data->client_stats);

  destroy_dynamic_memory(data->proxy_pids);
  destroy_dynamic_memory(data->proxy_stats);

  destroy_dynamic_memory(data->server_pids);
  destroy_dynamic_memory(data->server_stats);
  destroy_dynamic_memory(stats);
}

void destroy_shared_memory_buffers(struct main_data *data, struct communication_buffers *buffers){
  destroy_shared_memory(STR_SHM_MAIN_CLI_BUFFER, buffers->main_cli->operacoes, sizeof(struct operation) * data->max_ops);
  destroy_shared_memory(STR_SHM_MAIN_CLI_PTR, buffers->main_cli->ptr, sizeof(int) * data->max_ops);
  destroy_shared_memory(STR_SHM_MAIN_CLI_BUFFER, buffers->main_cli, sizeof(struct rnd_access_buffer));

  destroy_shared_memory(STR_SHM_CLI_PRX_BUFFER, buffers->cli_prx->operacoes, sizeof(struct operation) * data->max_ops);
  destroy_shared_memory(STR_SHM_CLI_PRX_BUFFER, buffers->cli_prx, sizeof(struct circular_buffer));

  destroy_shared_memory(STR_SHM_PRX_SRV_BUFFER, buffers->prx_srv->operacoes, sizeof(struct operation) * data->max_ops);
  destroy_shared_memory(STR_SHM_SRV_CLI_PTR, buffers->prx_srv->ptr, sizeof(int) * data->max_ops);
  destroy_shared_memory(STR_SHM_PRX_SRV_BUFFER, buffers->prx_srv, sizeof(struct rnd_access_buffer));

  destroy_shared_memory(STR_SHM_SRV_CLI_BUFFER, buffers->srv_cli->operacoes, sizeof(struct operation) * data->max_ops);
  destroy_shared_memory(STR_SHM_SRV_CLI_BUFFER, buffers->srv_cli, sizeof(struct circular_buffer));

  destroy_shared_memory(STR_SHM_RESULTS, data->results, sizeof(struct operation) * data->max_ops);
  destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));
}

void destroy_semaphores(struct semaphores *sems){
  semaphore_destroy(STR_SEM_MAIN_CLI_FULL, sems->main_cli->full);
  semaphore_destroy(STR_SEM_CLI_PRX_FULL, sems->cli_prx->full);
  semaphore_destroy(STR_SEM_PRX_SRV_FULL, sems->prx_srv->full);
  semaphore_destroy(STR_SEM_SRV_CLI_FULL, sems->srv_cli->full);

  semaphore_destroy(STR_SEM_MAIN_CLI_EMPTY, sems->main_cli->empty);
  semaphore_destroy(STR_SEM_CLI_PRX_EMPTY, sems->cli_prx->empty);
  semaphore_destroy(STR_SEM_PRX_SRV_EMPTY, sems->prx_srv->empty);
  semaphore_destroy(STR_SEM_SRV_CLI_EMPTY, sems->srv_cli->empty);
  
  semaphore_destroy(STR_SEM_MAIN_CLI_MUTEX, sems->main_cli->mutex);
  semaphore_destroy(STR_SEM_CLI_PRX_MUTEX, sems->cli_prx->mutex);
  semaphore_destroy(STR_SEM_PRX_SRV_MUTEX, sems->prx_srv->mutex);
  semaphore_destroy(STR_SEM_SRV_CLI_MUTEX, sems->srv_cli->mutex);

  destroy_dynamic_memory(sems->main_cli);
  destroy_dynamic_memory(sems->cli_prx);
  destroy_dynamic_memory(sems->prx_srv);
  destroy_dynamic_memory(sems->srv_cli);

  semaphore_destroy(STR_SEM_RESULTS_MUTEX, sems->results_mutex);
  destroy_dynamic_memory(sems);
}

void wakeup_processes(struct main_data *data, struct semaphores *sems){
  semaphore_mutex_unlock(sems->results_mutex);

  for (int x = 0; x < data->n_clients; x++){
    produce_end(sems->main_cli);
    produce_end(sems->cli_prx);
    produce_end(sems->prx_srv);
    produce_end(sems->srv_cli);
  }

  for (int x = 0; x < data->n_servers; x++){
    produce_end(sems->main_cli);
    produce_end(sems->cli_prx);
    produce_end(sems->prx_srv);
    produce_end(sems->srv_cli);
  }

  for (int x = 0; x < data->n_proxies; x++){
    produce_end(sems->main_cli);
    produce_end(sems->cli_prx);
    produce_end(sems->prx_srv);
    produce_end(sems->srv_cli);
  }

  produce_end(sems->main_cli);
  produce_end(sems->cli_prx);
  produce_end(sems->prx_srv);
  produce_end(sems->srv_cli);
}

int main(int argc, char *argv[]){
  //Aloca memoria
  data = create_dynamic_memory(sizeof(struct main_data));
  buffers = create_dynamic_memory(sizeof(struct communication_buffers));
  sems = create_dynamic_memory(sizeof(struct semaphores));
  logs = create_dynamic_memory(sizeof(struct estrutura_logs));
  stats = create_dynamic_memory(sizeof(struct info_stats));
  stats->statistics_filename = create_dynamic_memory(sizeof(char));
  alarmes = create_dynamic_memory(sizeof(struct info_alarme));
  alarmes->espaco_tempo = create_dynamic_memory(sizeof(int));
  cria_log(logs);

  //le ficheiro de entrada
  main_args(argc, argv, data);

  //Continua alocacao de memoria
  create_dynamic_memory_buffers(data);
  create_shared_memory_buffers(data, buffers);
  create_semaphores(data, sems);

  //User interaction
  launch_processes(buffers, data, sems);
  user_interaction(buffers, data, sems);
  stop_execution(data, buffers, sems);
}

