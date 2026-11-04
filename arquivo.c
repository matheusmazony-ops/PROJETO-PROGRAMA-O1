#include "arquivo.h"
#include "fraudes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define NUM_CATEGORIAS 14

// ARQUIVO PARA LER A TABELA HASH E ARMAZENAR OS CARTÕES NOS DEVIDOS ARQUIVOS //

void ler_numeros_cartoes(Tabela_hash *tabela,char numeros_cartoes[tam_tabela][50],int *quantidade_cartoes){
    // FUNCAO QUE LER AS GAVETAS DA TABELA HASH PARA ARMAZENAR EM UMA MATRIZ OS NUMEROS DOS CARTOES

    for (int i = 0 ; i < tam_tabela ; i++){
        if (tabela -> tabela_hash[i] != NULL){
            strcpy(numeros_cartoes[*quantidade_cartoes],tabela->tabela_hash[i]->numero_cartao);
            (*quantidade_cartoes)++;
        }
    }

    if (*quantidade_cartoes > tam_tabela){
        printf("Ha mais cartoes do que a memoria permite!\nSó foram armazenados %d cartoes\n", (*quantidade_cartoes));
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

static const char *nomes_categorias[NUM_CATEGORIAS + 1] = {
    "", "entertainment", "food_dining", "gas_transport", "grocery_net",
    "grocery_pos", "health_fitness", "home", "kids_pets", "misc_net",
    "misc_pos", "personal_care", "shopping_net", "shopping_pos", "travel"
};

void relatorio_cartao(Tabela_hash *tabela){
    char numero[32];
    printf("\nDigite o numero do cartao: ");
    scanf("%31s", numero);

    int qtd[NUM_CATEGORIAS + 1] = {0};
    double total[NUM_CATEGORIAS + 1] = {0.0};
    int encontrou = 0;

    for (NoTransacao *atual = tabela->tabela_hash[funcao_hash(numero)]; atual != NULL; atual = atual->proximo){
        if (strcmp(atual->numero_cartao, numero) == 0){
            encontrou = 1;
            if (atual->categoria >= 1 && atual->categoria <= NUM_CATEGORIAS){
                qtd[atual->categoria]++;
                total[atual->categoria] += atual->valor;
            }
        }
    }

    if (!encontrou){
        printf("\nCartao nao encontrado nas faturas carregadas!\n");
        return;
    }

    printf("\nNum. Cartao: %s\n", numero);
    printf("------------------------------------------------------\n");
    printf("%-20s %-16s %s\n", "Categoria", "Num. de compras", "Valor total");
    printf("------------------------------------------------------\n");
    for (int i = 1; i <= NUM_CATEGORIAS; i++){
        if (qtd[i] > 0){
            printf("%-20s %-16d %.2f\n", nomes_categorias[i], qtd[i], total[i]);
        }
    }
}