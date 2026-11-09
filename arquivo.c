#include "arquivo.h"
#include "fraudes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// ARQUIVO PARA LER A TABELA HASH E ARMAZENAR OS CARTÕES NOS DEVIDOS ARQUIVOS //

void ler_numeros_cartoes(Tabela_hash *tabela, char numeros_cartoes[max_cartoes][50], int *quantidade_cartoes){
    for (int i = 0 ; i < tam_tabela ; i++){
        NoTransacao *atual = tabela->tabela_hash[i];

        while (atual != NULL) {
            // Verifica se o número deste cartão já está cadastrado no vetor
            int ja_existe = 0;
            for (int j = 0; j < *quantidade_cartoes; j++) {
                if (strcmp(numeros_cartoes[j], atual->numero_cartao) == 0) {
                    ja_existe = 1;
                    break;
                }
            }

            // Se for um cartão novo:
            if (ja_existe == 0) {
                // VERIFICAÇÃO DOS 75 CARTÕES: Se atingiu 75, trava a adição!
                if (*quantidade_cartoes >= max_cartoes) {
                    printf("\n[AVISO] Limite maximo de %d cartoes unicos atingido!\n", max_cartoes);
                    printf("Novos cartoes alem dos %d primeiros serao ignorados.\n", max_cartoes);
                    return; // Interrompe a busca de novos cartões
                }
                
                // Salva o cartão no vetor
                strcpy(numeros_cartoes[*quantidade_cartoes], atual->numero_cartao);
                (*quantidade_cartoes)++;
            }

            atual = atual->proximo;
        }
    }
}


void criar_arquivos(char numeros_cartoes[tam_tabela][50],int quantidade_cartoes){
    // FUNCAO PARA CRIAR OS ARQUIVOS E DEIXAR NA MEMORIA DO DISCO

    char nome_arquivo[60];

    for (int i = 0 ; i < quantidade_cartoes ; i++){

        snprintf(nome_arquivo , sizeof(nome_arquivo) , "%s.csv" ,numeros_cartoes[i]);

        FILE *arquivo_criar;

        arquivo_criar = fopen(nome_arquivo , "w");

        if (arquivo_criar==NULL){
            puts("Erro na criação do arquivo.");
            continue;
        }

        fclose(arquivo_criar);

    }
}


void preencher_arquivos(Tabela_hash *tabela,char numeros_cartoes[tam_tabela][50],int quantidade_cartoes){
    for (int i =0;i<quantidade_cartoes;i++){

        char nome_arquivo[60];

        // Conectar seu código com o seu arquivo de disco
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.csv", numeros_cartoes[i]);

        FILE *arquivo_temp;
        arquivo_temp=fopen(nome_arquivo, "w");

        if (arquivo_temp==NULL){
            printf("ERRO AO ABRIR ARQUIVO DE NOME '%s'",numeros_cartoes[i]);
            continue;
        }

        int gaveta = funcao_hash (numeros_cartoes[i]);

        NoTransacao *inicio=tabela->tabela_hash[i];
        NoTransacao *atual=tabela->tabela_hash[gaveta];

        // VARIÁVEIS DE CACHE PARA A FRAUDE B (Reiniciam a cada cartão)
        int mes_cache = -1;
        int ano_cache = -1;
        double soma_cache = 0.0;
        int qtd_cache = 0;


        // São variáveis temporárias para armazenar     
        while(atual!=NULL){
            if (strcmp(atual->numero_cartao,numeros_cartoes[i])==0){

                aplicar_epoch_no_arquivo(atual->data,arquivo_temp);

                fprintf(arquivo_temp,"%d,%.2f,", atual->categoria, atual->valor);

                // Incluir, além da data , dos valores e categorias nos arquivos CSV , os valores e indicadores de fraudes dos tipos A, B e C
                verificar_fraude_tipo_A(inicio,atual,numeros_cartoes[i],arquivo_temp);
                verificar_fraude_tipo_B(inicio,atual,&mes_cache,&ano_cache,&soma_cache,&qtd_cache,arquivo_temp) ;
                verificar_fraude_tipo_C(atual,arquivo_temp);
        }

        atual = atual->proximo;
        }

        fclose(arquivo_temp);
    }
}