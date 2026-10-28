#include <stdio.h>
#include "hashtable.h"
#include "fraudes.h"
#include "arquivo.h"
#include "interface.h"


int main(){
    FILE *arquivocsv = fopen("1_jan2019.csv","r");

    if (arquivocsv == NULL){
        printf("Erro ao abrir o arquivo!");
        return 0;
    }
    Tabela_hash *tabela = criar_tabela();

    if (tabela != NULL){
        ler_e_preencher_tabela(arquivocsv, tabela);
    } else{
        printf("Erro ao alocar memoria");
    }


    ler_e_preencher_tabela(arquivocsv,tabela);

    char numeros_cartoes[tam_tabela][50];
    int quantidade_cartoes=0;

    ler_numeros_cartoes(tabela,numeros_cartoes,&quantidade_cartoes);

    for (int i=0;i<quantidade_cartoes;i++){
        printf("Cartao [%d] : %s\n",i,numeros_cartoes[i]);
    }

    criar_arquivos(numeros_cartoes,quantidade_cartoes);
    
    return 0;

}
