/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/main.h"
#include "../include/memory.h"
#include "../include/configuration.h"
#include "../include/log.h"
#include "../include/stats.h"
#include "../include/soalarm.h"

#define MAXLINHA 20

void lerFicheiro(int argc, char * argv[], struct main_data * data, struct estrutura_logs * logs, struct info_stats * stats) {
    char linha[MAXLINHA];
    int count = 1;
    FILE * txtfile;

    if (argc < 2) {
      fprintf(stderr, "Por favor especifique o nome do ficheiro.\n");
      //exit(0);
    }

    txtfile = fopen(argv[1], "r");
    if (txtfile == NULL) {
      fprintf(stderr, "O ficheiro nao pode ser lido pois nao existe.\n");
      // exit(1);
    }

    while (fgets(linha, MAXLINHA, txtfile) != NULL) {
      int numero = 0;
      char nomeFicheiro[40];
       memset(nomeFicheiro, 0, sizeof(nomeFicheiro));
      if (count < 7) {
        sscanf(linha, "%d", &numero);
      } else if(count == 7) {
        sscanf(linha, "%s", logs->nome_ficheiro);
      }else{
        sscanf(linha, "%s", stats->statistics_filename);
      }

      switch (count) {
      case 1:
        data -> max_ops = numero;
        break;
      case 2:
        data -> buffers_size = numero;
        break;
      case 3:
        data -> n_clients = numero;
        break;
      case 4:
        data -> n_proxies = numero;
        break;
      case 5:
        data -> n_servers = numero;
        break;
      case 6:
        alarmes->espaco_tempo = numero;
        break;
      }
      count++;
    }
    fclose(txtfile);
    }