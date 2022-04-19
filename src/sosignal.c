/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "../include/memory-private.h"
#include "../include/sotime.h"
#include "../include/stats.h"
#include "../include/main.h"
#include "../include/soalarm.h"

void signal_handling(int sig){
  if(sig == SIGINT){
    signal(SIGINT, signal_handling);
    stop_execution(data, buffers, sems);
  }else if(sig == SIGALRM){
    signal(SIGALRM,signal_handling);
    
    alarm(alarmes->espaco_tempo);
    for(int x = 0; x < data->max_ops && data->results[x].status; x++){
      printf("op: %d of %d status: %c start: %d client: %d client_time: %d proxy: %d proxy_time: %d server: %d server_time: %d end: %d\n", x, data->max_ops, data -> results[x].status ,transforma_nano(&data -> results[x].start_time),
      data -> results[x].client, transforma_nano(&data -> results[x].client_time),
      data -> results[x].proxy, transforma_nano(&data -> results[x].proxy_time),
      data -> results[x].server, transforma_nano(&data -> results[x].server_time),
      transforma_nano(&data -> results[x].end_time));
    }
    
  }
}