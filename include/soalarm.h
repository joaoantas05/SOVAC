/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#ifndef SOALARM_H_GUARD
#define SOALARM_H_GUARD

#include <time.h>

//estrutura que guarda as informações dos alarmes
struct info_alarme{
  int *espaco_tempo;
};

//faz print das estatisticas
void mostra_info_alarm();

#endif