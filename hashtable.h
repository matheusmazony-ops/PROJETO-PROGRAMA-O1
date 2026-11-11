#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>

#define tam_tabela  50021 // GAVETAS DA TABELA HASH
#define max_cartoes 10000   // QUANTIDADE MAXIMA DE CARTOES DO PROGRAMA
#define max_arquivos 50 // QUANTIDADE MAXIMA DE ARQUIVOS LIDOS NO PROGRAMA

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

Tabela_hash* ler_arquivo_e_preencher_tabela(Tabela_hash* tabela, int *quantidade_cartoes_lidos,int *quantidade_arquivos_lidos, char arquivos_lidos[max_arquivos][50]);

int funcao_hash(const char *numero_cartao);

void liberar_tabela(Tabela_hash *tabela);

void ordenar_gavetas_tabela_hash(Tabela_hash *tabela,char numeros_cartoes[max_cartoes][50],int quantidade_cartoes);

#endif
