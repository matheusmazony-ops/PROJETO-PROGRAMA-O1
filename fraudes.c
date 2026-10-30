#include "fraudes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <math.h>


// ARQUIVOS PARA VERIFICACOES DE FRAUDES NO MOMENTO DE PREENCHER OS ARQUIVOS DOS CARTOES

void obter_mes_ano(long epoch_time, int *mes, int *ano){
    time_t tempo_epoch = (time_t) epoch_time;
    struct tm *info = localtime(&tempo_epoch);
    if(info != NULL){
        // Guardar o mes de 0 ate 11
        *mes = info->tm_mon + 1;
        // Guardar o ano a partir de 1900
        *ano = info->tm_year + 1900;
    } else{
        printf("Nao foi possivel converter o Epoch Time\n");
    }
}


void calcular_totais_fatura(NoTransacao *inicio, int mes, int ano, double *soma_fatura, int *qtd_fatura){
    // Valor da memória apontada usando o operador de desreferenciação, nesse caso *
    *soma_fatura = 0.0;
    *qtd_fatura = 0;

    NoTransacao *atual = inicio;
    int mes_atual, ano_atual;

    while(atual != NULL){
        // transforma o Epoch Time em mês e ano
        obter_mes_ano(atual->data, &mes_atual, &ano_atual);

        // Filtra a transação que pertece a uma fatura específica, ignorando a transação de outros meses e anos
        if(mes_atual == mes && ano_atual == ano){
            *soma_fatura += atual->valor;
            (*qtd_fatura)++;
        }

        atual = atual->proximo;
    }
}


void verificar_fraude_tipo_B(NoTransacao *inicio, int mes, int ano){
    double soma_fatura;
    int qtd_fatura;

    calcular_totais_fatura(inicio, mes, ano, &soma_fatura, &qtd_fatura);

    if (qtd_fatura <= 1){
        // Se essa condicao for aceita, o cliente fez 0 ou 1 compra no mês
        return;
    }

    NoTransacao *atual = inicio;
    int mes_atual, ano_atual;

    while(atual != NULL){
        obter_mes_ano(atual->data, &mes_atual, &ano_atual);

        if(mes_atual == mes && ano_atual == ano){
            // Encontrando o valor da média de outras transações
            double soma_outras = soma_fatura - atual->valor;
            int qtd_outras = qtd_fatura - 1;
            double media_outras = soma_outras / qtd_outras;

            if(atual->valor >= 3.0 * media_demais){
                atual->fraude = 'Y';
                atual->tipo_fraude = 'B';
            }
        }

        atual = atual->proximo;
    }

}


DataHora epoch_para_data(int64_t epoch){
    // FUNCAO TRANSFORMAR NUMERO EPOCH EM DATA
    DataHora resultado;

    /* ---------- PASSO 1: separar dias e segundos ---------- */
    int64_t dias_desde_1970 = epoch / 86400;
    int64_t segundos_do_dia = epoch % 86400;

    /* epoch negativo (antes de 1970): ajusta para o resto ficar positivo */
    if (segundos_do_dia < 0) {
        segundos_do_dia += 86400;
        dias_desde_1970--;
    }

    /* ---------- PASSO 2: hora, minuto e segundo ---------- */
    resultado.hora    = (int)(segundos_do_dia / 3600);
    resultado.minuto  = (int)((segundos_do_dia % 3600) / 60);
    resultado.segundo = (int)(segundos_do_dia % 60);

    /* ---------- PASSO 3: dia da semana ---------- */
    /* 01/01/1970 foi quinta-feira (4) */
    resultado.dia_semana = (int)(((dias_desde_1970 % 7) + 11) % 7);

    /* ---------- PASSO 4: dias -> data ---------- */
    /* Muda o ponto de partida de 01/01/1970 para 01/03/0000,
       para que o dia bissexto fique no FINAL do ano */
    int64_t dias_desde_marco_0000 = dias_desde_1970 + 719468;

    /* Quantos ciclos completos de 400 anos já passaram */
    int64_t ciclos_de_400_anos;
    if (dias_desde_marco_0000 >= 0)
        ciclos_de_400_anos = dias_desde_marco_0000 / 146097;
    else
        ciclos_de_400_anos = (dias_desde_marco_0000 - 146096) / 146097;

    /* Em que dia do ciclo atual estamos (0 a 146096) */
    int64_t dia_no_ciclo = dias_desde_marco_0000 - ciclos_de_400_anos * 146097;

    /* Em que ano do ciclo atual estamos (0 a 399),
       descontando os dias extras dos anos bissextos */
    int64_t ano_no_ciclo = (dia_no_ciclo
                            - dia_no_ciclo / 1460
                            + dia_no_ciclo / 36524
                            - dia_no_ciclo / 146096) / 365;

    /* Ano "bruto" (ainda com o ano começando em março) */
    int64_t ano_comecando_em_marco = ano_no_ciclo + ciclos_de_400_anos * 400;

    /* Em que dia do ano estamos (0 = 1º de março, 365 = 29 de fevereiro) */
    int64_t dia_no_ano = dia_no_ciclo
                         - (365 * ano_no_ciclo + ano_no_ciclo / 4 - ano_no_ciclo / 100);

    /* Mês contado a partir de março (0 = março, 11 = fevereiro) */
    int64_t mes_comecando_em_marco = (5 * dia_no_ano + 2) / 153;

    /* Dia do mês (1 a 31) */
    resultado.dia = (int)(dia_no_ano - (153 * mes_comecando_em_marco + 2) / 5 + 1);

    /* Converte o mês de "março = 0" para o normal "janeiro = 1" */
    if (mes_comecando_em_marco < 10)
        resultado.mes = (int)(mes_comecando_em_marco + 3);   /* mar..dez */
    else
        resultado.mes = (int)(mes_comecando_em_marco - 9);   /* jan..fev */

    /* Janeiro e fevereiro pertencem ao ano seguinte nessa contagem */
    resultado.ano = ano_comecando_em_marco + (resultado.mes <= 2);

    return resultado;
}


void aplicar_epoch_no_arquivo(int64_t epoch ,FILE *arquivo_temp){
    //FUNCAO PARA APLICAR AS DATAS NO ARQUIVO DO CARTAO

    DataHora resultado = epoch_para_data(epoch);

    fprintf(arquivo_temp,"%04lld-%02d-%02d %02d:%02d:%02d (UTC) ",
           (long long)resultado.ano, resultado.mes, resultado.dia,
           resultado.hora, resultado.minuto, resultado.segundo);

}

void verificar_fraude_tipo_C(NoTransacao *atual,FILE *arquivo_temp){
    // FUNCAO PARA VERIFICAR FRAUDE SE A DISTANCIA ENTRE COMPRA FOR MAIOR QUE 78KM
    double raio_terra = 6371.0;

    double latitude_residencia = conversao_em_radianos(atual->latitude_residencia);
    double longitude_residencia = conversao_em_radianos(atual->longitude_residencia);

    double latitude_compra = conversao_em_radianos(atual->latitude_compra);
    double longitude_compra = conversao_em_radianos(atual->longitude_compra);

    double diferenca_latitude = latitude_compra-latitude_residencia;
    double diferenca_longitude = longitude_compra-longitude_residencia;

    double fator_abertura_angular = sin(diferenca_latitude / 2.0) * sin(diferenca_latitude / 2.0) +
                              cos (latitude_residencia) * cos(latitude_compra) *
                              sin(diferenca_longitude / 2.0) * sin(diferenca_longitude / 2.0);

    double arco_da_curva = 2.0 * asin(sqrt(fator_abertura_angular));

    double distancia_km= raio_terra * arco_da_curva;

    if (distancia_km>78){
        fprintf(arquivo_temp,"Y\n");
    } else {
        fprintf(arquivo_temp,"N\n");
    }
}

double conversao_em_radianos(double graus) {
    // FUNCAO PARA CONVERTER OS NUMEROS DE LATITUDE E LONGITUDE EM RADIANOS
    return graus * (PI / 180.0);
}
