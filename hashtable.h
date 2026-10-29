#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>

#define tam_tabela 17

typedef struct noTransacao{
    long data;
    char numero_cartao[30];
    short categoria;
    float valor;
    char genero;
    double latitude_residencia;
    double longitude_residencia;
    double latitude_compra;
    double longitude_compra;
    struct noTransacao *proximo;
} NoTransacao;

typedef struct{
    NoTransacao *tabela_hash[tam_tabela];
} Tabela_hash;


Tabela_hash* criar_tabela(void);

Tabela_hash* ler_arquivo_e_preencher_tabela(Tabela_hash* tabela,int *quantidade_arquivos_lidos,char arquivos_lidos[tam_tabela][50]);

int funcao_hash(const char *numero_cartao);

void liberar_tabela(Tabela_hash *tabela);

#endif
