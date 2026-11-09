#include "arquivo.h"
#include "fraudes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// ARQUIVO DESTINADO AO RELATORIO DAS COMPRAS DE ALGUM CARTÃO 

#define NUM_CATEGORIAS 14

static const char *nomes_categorias[NUM_CATEGORIAS + 1] = {
    "", 
    "entertainment", 
    "food_dining", 
    "gas_transport", 
    "grocery_net",
    "grocery_pos", 
    "health_fitness", 
    "home", 
    "kids_pets", 
    "misc_net",
    "misc_pos", 
    "personal_care", 
    "shopping_net", 
    "shopping_pos", 
    "travel"
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