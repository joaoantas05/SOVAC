/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include "../include/server.h"
#include "../include/synchronization.h"
#include "../include/sotime.h"

int execute_server(int server_id, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    struct operation *operacao = create_dynamic_memory(sizeof(struct operation));
    int count = 0;

    while (1){
        if (*data->terminate == 1){
            destroy_dynamic_memory(operacao);
            return count;
        }

        server_receive_operation(operacao, buffers, data, sems);
        if (!*data->terminate && operacao->id != -1){
          server_process_operation(operacao, server_id, &count);
          server_send_answer(operacao, buffers, data, sems);
                
        }else if(!*data->terminate && operacao->id == -1){
          server_process_operation(operacao, server_id, &count);
        }
    }
}

void server_receive_operation(struct operation *op, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
    consume_begin(sems->prx_srv);
    if (!*data->terminate){
        read_rnd_access_buffer(buffers->prx_srv, data->buffers_size, op);
        consume_end(sems->prx_srv);
    }
}

void server_process_operation(struct operation *op, int server_id, int *counter){
    op->status = 'S';
    op->server = server_id;
    adicionar_tempo(&(op->server_time));

    if (op->id != -1){
      *counter += 1;
    }
}

void server_send_answer(struct operation *op, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
    produce_begin(sems->srv_cli);
    write_circular_buffer(buffers->srv_cli, data->buffers_size, op);
    produce_end(sems->srv_cli);
}