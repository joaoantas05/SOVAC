/*
* Elementos do grupo 24:
* Tiago Águeda a22001757
* João Antas a22002629
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "../include/sotime.h"


void adicionar_tempo_tostring(struct timespec *tp, char *ts){
  char buffer[22],time2[32];
  strftime(buffer, 22, "%D %T", gmtime(&tp->tv_sec));
  snprintf(time2,22, "%s.%09ld", buffer, tp->tv_nsec);
  strcpy(ts, time2);
}


void adicionar_tempo(struct timespec * tempo) {
    clock_gettime(CLOCK_REALTIME, tempo);
}

int diff_timespec(const struct timespec *time1, const struct timespec *time0) {
  
  return (time1->tv_sec - time0->tv_sec) * 1000000L + (time1->tv_nsec - time0->tv_nsec)/ 1000L;
}

int transforma_nano(struct timespec *time1) {
  return abs(time1->tv_sec * 1000000000 + time1->tv_nsec);
}
