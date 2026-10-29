#include "arquivo.h"
#include "fraudes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

    char nome_arquivo[30];

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

        FILE *arquivo_temp;
        arquivo_temp=fopen(numeros_cartoes[i],"w");

        if (arquivo_temp==NULL){
            printf("ERRO AO ABRIR ARQUIVO DE NOME '%s'",numeros_cartoes[i]);
            continue;
        }
        
        int gaveta = funcao_hash (numeros_cartoes[i]) ;

        NoTransacao *atual=tabela->tabela_hash[gaveta];
        
        while(atual!=NULL){
            DataHora numero_temp= epoch_para_data(atual->data);

            
            aplicar_epoch(numero_epoch,arquivo_temp);
            
            
                

        }


        

    }

}


