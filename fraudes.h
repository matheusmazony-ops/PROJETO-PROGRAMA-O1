#ifndef FRAUDES_H
#define FRAUDES_H
#include <stdint.h>
#include <math.h>

#include "hashtable.h"
#include "arquivo.h"

#define PI 3.14159265358979323846

typedef struct {
    int64_t ano;
    int mes;          /* 1 a 12 */
    int dia;          /* 1 a 31 */
    int hora;         /* 0 a 23 */
    int minuto;       /* 0 a 59 */
    int segundo;      /* 0 a 59 */
    int dia_semana;   /* 0 = domingo ... 6 = sábado */
} DataHora;

void aplicar_epoch_no_arquivo(int64_t epoch ,FILE *arquivo_temp);

void obter_mes_ano(long epoch_time, int *mes, int *ano);

void calcular_totais_fatura(NoTransacao *inicio, int mes, int ano, double *soma_fatura, int *qtd_fatura);

DataHora epoch_para_data(int64_t epoch);

void verificar_fraude_tipo_C(NoTransacao *atual,FILE *arquivo_temp);

double conversao_em_radianos(double graus);

#endif
