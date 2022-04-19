/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include "../include/synchronization.h"
#include "../include/client.h"
#include "../include/sotime.h"

int execute_client(int client_id, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
  struct operation *operacao = create_dynamic_memory(sizeof(struct operation));
  int count = 0;

  while (1){
    if (*data->terminate){
      destroy_dynamic_memory(operacao);
      return count;
    }

    client_get_operation(operacao, buffers, data, sems);

    if(operacao->id == -1 && !*data->terminate){
      client_process_operation(operacao, client_id, &count);
    }else if ( !*data->terminate && operacao->id != -1 ){
      client_process_operation(operacao, client_id, &count);
      client_send_operation(operacao, buffers, data, sems);
    }

    client_receive_answer(operacao, buffers, data, sems);

    if (!*data->terminate && operacao->id != -1){
      client_process_answer(operacao, data, sems);
    }
  }
}

void client_get_operation(struct operation *op, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
  consume_begin(sems->main_cli);
  if (!*data->terminate){
    read_rnd_access_buffer(buffers->main_cli, data->buffers_size, op);
    consume_end(sems->main_cli);
  }
}

void client_process_operation(struct operation *op, int client_id, int *count){
  op->status = 'C';
  op->client = client_id;
  adicionar_tempo(&(op->client_time));

  if (op->id != -1){
    *count += 1;
  }
}

void client_send_operation(struct operation *op, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
  produce_begin(sems->cli_prx);
  write_circular_buffer(buffers->cli_prx, data->buffers_size, op);
  produce_end(sems->cli_prx);
}

void client_receive_answer(struct operation *op, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
  consume_begin(sems->srv_cli);
  if (!*data->terminate){
    read_circular_buffer(buffers->srv_cli, data->buffers_size, op);
    consume_end(sems->srv_cli);
  }
}

void client_process_answer(struct operation *op, struct main_data *data, struct semaphores *sems){
  if (data->results[op->id].status != 'S'){
    semaphore_mutex_lock(sems->results_mutex);
    adicionar_tempo(&(op->end_time));
    data->results[op->id] = *op;
    semaphore_mutex_unlock(sems->results_mutex);
  }
}