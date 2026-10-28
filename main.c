#include <stdio.h>
#include "hashtable.h"
#include "fraudes.h"
#include "arquivo.h"
#include "interface.h"


int main(){

    Tabela_hash *tabela = criar_tabela();

    if (tabela==NULL){
        printf("Erro ao alocar a tabela hash!\n");
        return EXIT_FAILURE;
    }
    
    char arquivos_lidos[tam_tabela][50];
    int quantidade_arquivos_lidos=0;

    char numeros_cartoes[tam_tabela][50];
    int quantidade_cartoes=0;

    int user;

    do {
        user=menu();
        switch(user){
            case 0:
                printf("\nPrograma encerrado!\n\n");
                break;
            
            case 1:
                ler_arquivo_e_preencher_tabela(tabela,&quantidade_arquivos_lidos,arquivos_lidos);

                ler_numeros_cartoes(tabela,numeros_cartoes,&quantidade_cartoes);
                
                criar_arquivos(numeros_cartoes,quantidade_cartoes);
                
                /*for (int i=0;i<quantidade_cartoes;i++){
                        printf("Cartao [%d] : %s\n",i,numeros_cartoes[i]);
                    }
                */ 
                break;
            
            default:
                printf("\nOpcao invalida! Digite novamente!\n");
                break;
        }
        
    } while(user!=0);

    return 0;

}
