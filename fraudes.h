#ifndef FRAUDES_H
#define FRAUDES_H

#include "hashtable.h"

typedef struct {
    int64_t ano;
    int mes;          /* 1 a 12 */
    int dia;          /* 1 a 31 */
    int hora;         /* 0 a 23 */
    int minuto;       /* 0 a 59 */
    int segundo;      /* 0 a 59 */
    int dia_semana;   /* 0 = domingo ... 6 = sábado */
} DataHora;

void aplicar_epoch(DataHora epoch_para_data(int64_t),FILE *arquivo_temp);

void obter_mes_ano(long epoch_time, int *mes, int *ano);

DataHora epoch_para_data(int64_t epoch);


#endif
