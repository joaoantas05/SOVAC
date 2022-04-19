/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../include/process.h"
#include "../include/server.h"
#include "../include/client.h"
#include "../include/proxy.h"
#include "../include/sosignal.h"


int launch_process(int process_id, int process_code, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems){
    int process_pid = fork();
    signal(SIGINT, SIG_IGN);
    
    if (!process_pid){
        switch (process_code){
        case 0:
            exit(execute_client(process_id, buffers, data, sems));
        case 1:
            exit(execute_proxy(process_id, buffers, data, sems));
        case 2:
            exit(execute_server(process_id, buffers, data, sems));
        }
    }else{
      return -1;
    }
    return process_pid;
}

int wait_process(int process_id){
    int resultado;
    waitpid(process_id, &resultado, WCONTINUED);

    if (!WIFEXITED(resultado)){
      return 0;
    }
    return WEXITSTATUS(resultado);
}