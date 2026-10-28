#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "hashtable.h"




void ler_numeros_cartoes(Tabela_hash *tabela,char numeros_cartoes[tam_tabela][50],int *quantidade_cartoes);

void criar_arquivos(char numeros_cartoes[tam_tabela][50],int quantidade_cartoes);

void preencher_arquivos(Tabela_hash *tabela,char numeros_cartoes[tam_tabela][50],int quantidade_cartoes);


#endif