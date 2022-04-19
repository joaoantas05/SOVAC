/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#ifndef CONFIGURATION_H_GUARD
#define CONFIGURATION_H_GUARD

#include "log.h"
#include "main.h"
#include "stats.h"

// Função que guarda as informações lidas do ficheiro de entrada
void lerFicheiro(int argc, char * argv[], struct main_data * data, struct estrutura_logs * logs, struct info_stats * stats);

#endif