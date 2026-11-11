#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "hashtable.h"
#include "fraudes.h"
#define PI 3.14159265358979323846

// COMO DEVE FICAR (Correto - trocado max_cartoes por max_arquivos):
Tabela_hash* ler_arquivo_e_preencher_tabela(Tabela_hash* tabela, int *quantidade_cartoes_lidos, int *quantidade_arquivos_lidos, char arquivos_lidos[max_arquivos][50]);

void ler_numeros_cartoes(Tabela_hash *tabela, char numeros_cartoes[max_cartoes][50], int *quantidade_cartoes);

void criar_arquivos(char numeros_cartoes[max_cartoes][50], int quantidade_cartoes);

void preencher_arquivos(Tabela_hash *tabela, char numeros_cartoes[max_cartoes][50], int quantidade_cartoes);

#endif