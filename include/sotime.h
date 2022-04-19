/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#ifndef SOTIME_H_GUARD
#define SOTIME_H_GUARD


//Converte os tempos para strings
void adicionar_tempo_tostring(struct timespec *time_flag, char *time_string);

//Função que cria tempos
void adicionar_tempo(struct timespec * tempo);

//Calcula a diferença entre 2 datas
int diff_timespec(const struct timespec *time1, const struct timespec *time0);

//transforma um tempo para nano-segundos
int transforma_nano(struct timespec *time1);


#endif