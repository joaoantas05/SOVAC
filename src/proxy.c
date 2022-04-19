/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include "../include/proxy.h"
#include "../include/synchronization.h"
#include "../include/sotime.h"


int execute_proxy(int proxy_id, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
    
    struct operation *operacao = create_dynamic_memory(sizeof(struct operation));
    int count = 0;

    while (1){
        if (*data->terminate != 0){
          destroy_dynamic_memory(operacao);
          return count;
        }else{
            proxy_receive_operation(operacao, buffers, data, sems);
            if (!*data->terminate && operacao->id != -1){
              proxy_process_operation(operacao, proxy_id, &count);
              proxy_forward_operation(operacao, buffers, data, sems);
                
            }else if(!*data->terminate && operacao->id == -1){
              proxy_process_operation(operacao, proxy_id, &count);
            }
        }
    }
}

void proxy_receive_operation(struct operation *op, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
    consume_begin(sems->cli_prx);
    if (!*data->terminate){
        read_circular_buffer(buffers->cli_prx, data->buffers_size, op);
        consume_end(sems->cli_prx);
    }
}

void proxy_process_operation(struct operation *op, int proxy_id, int *counter){
    op->status = 'P';
    op->proxy = proxy_id;
    adicionar_tempo(&(op->proxy_time));


    if (op->id != -1){
        *counter += 1;
    }
}

void proxy_forward_operation(struct operation *op, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
    produce_begin(sems->prx_srv);
    write_rnd_access_buffer(buffers->prx_srv, data->buffers_size, op);
    produce_end(sems->prx_srv);
}