#include "hashtable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// ARQUIVO PARA CRIAR, LER E PREENCHER A TABELA HASH //

Tabela_hash* criar_tabela() {
    Tabela_hash *tabela = (Tabela_hash*) malloc(sizeof(Tabela_hash));

    if (tabela != NULL) {
        for (int i = 0; i < tam_tabela; i++) {
            tabela->tabela_hash[i] = NULL;
        }
    }
    return tabela; // Retorna a tabela criada ou NULL se falhar a alocação
}

int funcao_hash(const char *numero_cartao) {
    unsigned int codigo = 0;
    int posicao_numero_cartao = 0;

    while (numero_cartao[posicao_numero_cartao] != '\0') {
        codigo = (unsigned char) numero_cartao[posicao_numero_cartao] + 31 * codigo;
        posicao_numero_cartao++;
    }

    return codigo % tam_tabela;
}

Tabela_hash* ler_arquivo_e_preencher_tabela(Tabela_hash* tabela,int *quantidade_cartoes_lidos,char arquivos_lidos[tam_tabela][50]) {

    printf("\nDigite o nome do arquivo que deseja ler (*OBS:Apenas o nome , sem o seu formato): ");
    scanf("%s",arquivos_lidos[*quantidade_cartoes_lidos]);

    char arquivo_lido_temp[50];
    snprintf(arquivo_lido_temp,sizeof(arquivo_lido_temp),"%s.csv",arquivos_lidos[*quantidade_cartoes_lidos]);

    for (int i=0;i<*quantidade_cartoes_lidos;i++){
        if (strcmp(arquivos_lidos[i],arquivo_lido_temp)==0){
            printf("\nO arquivo ja foi lido anteriormente!\n");
            return tabela;
        }
    }

    strcpy(arquivos_lidos[*quantidade_cartoes_lidos],arquivo_lido_temp);

    FILE *arquivocsv=fopen(arquivo_lido_temp,"r");

    if (arquivocsv==NULL){
        printf("\nErro ao abrir o arquivo , verifique se o nome do arquivo esta correto!\n");
        return tabela;
    }


    (*quantidade_cartoes_lidos)++;

    long data;
    char numero_cartao[32];
    short categoria;
    float valor;
    char genero;
    double latitude_residencia;
    double longitude_residencia;
    double latitude_compra;
    double longitude_compra;

    char cabecalho[100];
    // O '\n' no final limpa a quebra de linha do buffer
    fscanf(arquivocsv, "%[^\n]\n", cabecalho);

    while (fscanf(arquivocsv, " %ld,%31[^,],%hd,%f ,%c,%lf,%lf,%lf,%lf",
                  &data,
                  numero_cartao,
                  &categoria,
                  &valor,
                  &genero,
                  &latitude_residencia,
                  &longitude_residencia,
                  &latitude_compra,
                  &longitude_compra) == 9) {

        NoTransacao *novo = (NoTransacao*) malloc(sizeof(NoTransacao));

        if (novo == NULL) {
            puts("Erro de memória ao alocar nó!");
            return tabela;
        }

        novo->data = data;
        strcpy(novo->numero_cartao, numero_cartao);
        novo->categoria = categoria;
        novo->valor = valor;
        novo->genero = genero;
        novo->latitude_residencia = latitude_residencia;
        novo->longitude_residencia = longitude_residencia;
        novo->latitude_compra = latitude_compra;
        novo->longitude_compra = longitude_compra;

        novo->fraude = 'N';
        novo->tipo_fraude = 'X';

        int indice = funcao_hash(numero_cartao);

        novo->proximo = tabela->tabela_hash[indice];
        tabela->tabela_hash[indice] = novo;
    }

    fclose(arquivocsv);

    return tabela;
}

void ordenar_gavetas_tabela_hash(Tabela_hash *tabela,char numeros_cartoes[tam_tabela][50],int quantidade_cartoes){

    for (int i = 0; i < tam_tabela; i++){

        if (tabela->tabela_hash[i] != NULL){

            NoTransacao *lista_ordenada = NULL; // CRIA A GAVETA TEMPORARIA VAZIA PARA A TABELA HASH

            NoTransacao *inicio=tabela->tabela_hash[i];

            while(inicio!=NULL){
                NoTransacao *proximo=inicio->proximo;
                NoTransacao *anterior = NULL;
                NoTransacao *atual=lista_ordenada;

                while (atual!=NULL && atual->data <= inicio->data){
                    anterior=atual;
                    atual = atual -> proximo;
                }

                if (anterior==NULL){
                    inicio->proximo = lista_ordenada;
                    lista_ordenada = inicio;
                } else{
                    inicio -> proximo = atual;
                    anterior -> proximo = inicio;
                }
                inicio = proximo;

            }
            tabela->tabela_hash[i] = lista_ordenada;
        }
        else{
            continue;
        }
    }

}

void liberar_tabela(Tabela_hash *tabela) {
    if (tabela == NULL) return;

    for (int i = 0; i < tam_tabela; i++) {
        NoTransacao *atual = tabela->tabela_hash[i];
        while (atual != NULL) {
            NoTransacao *proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }

    free(tabela);
}
