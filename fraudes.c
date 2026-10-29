#include "fraudes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


void obter_mes_ano(long epoch_time, int *mes, int *ano){
    time_t tempo_epoch = (time_t) epoch_time;
    struct tm *info = localtime(&tempo_epoch);
    if(info != NULL){
        // Guardar o mês de 0 até 11
        *mes = info->tm_mon + 1;
        // Guardar o ano a partir de 1900
        *ano = info->tm_year + 1900;
    } else{
        printf("Não foi possível converter o Epoch Time\n");
    }

}
