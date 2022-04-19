/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/memory-private.h"
#include "../include/sotime.h"
#include "../include/stats.h"
#include "../include/main.h"


void stats_cria_ficheiro(struct estrutura_logs *logs, struct main_data *data, struct info_stats *stats){
  FILE *output_file = fopen(stats->statistics_filename, "w");
  fprintf(output_file, "Process Statistics:\n");
  for (int x = 0; x < data -> n_clients; x++) {
    fprintf(output_file,"Client %d received %d requests!\n", x, data -> client_stats[x]);
  }
  for (int x = 0; x < data->n_servers; x++) {
    fprintf(output_file,"Proxy %d forwarded %d requests!\n", x, data -> proxy_stats[x]);
  }
  for (int x = 0; x < data->n_proxies; x++) {
    fprintf(output_file,"Server %d responded to %d requests!\n", x, data -> server_stats[x]);
  }
  fprintf(output_file,"\n");
  fprintf(output_file, "Operation Statistics:\n");

  for(int x = 0; x < data -> max_ops && data->results[x].status == 'S'; x++ ){
    
    fprintf(output_file,"OP: %d\n", data -> results[x].id);
    fprintf(output_file,"Status: %c\n", data -> results[x].status);
    fprintf(output_file,"Client_id: %d\n", data -> results[x].client);
    fprintf(output_file,"Proxy_id: %d\n", data -> results[x].proxy);
    fprintf(output_file,"Server_id: %d\n", data -> results[x].server);
    
    char hora_inicio[22];
    char hora_cliente[22];
    char hora_proxy[22];
    char hora_server[22];
    char hora_end[22];
    adicionar_tempo_tostring(&data -> results[x].start_time, hora_inicio);
    fprintf(output_file, "Created: %s\n", hora_inicio);
    adicionar_tempo_tostring(&data -> results[x].client_time, hora_cliente);
    fprintf(output_file, "Client_time: %s\n", hora_cliente);
    adicionar_tempo_tostring(&data -> results[x].proxy_time, hora_proxy);
    fprintf(output_file, "Proxy_time: %s\n", hora_proxy);
    adicionar_tempo_tostring(&data -> results[x].server_time, hora_server);
    fprintf(output_file, "Server_time: %s\n", hora_server);
    adicionar_tempo_tostring(&data -> results[x].end_time, hora_end);
    fprintf(output_file, "Ended: %s\n", hora_end);
    fprintf(output_file, "Total Time(us): %d\n", diff_timespec(&data -> results[x].end_time, &data -> results[x].start_time));
    fprintf(output_file,"\n");
  }

  fclose(output_file);
}
