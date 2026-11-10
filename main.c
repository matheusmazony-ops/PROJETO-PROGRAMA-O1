#include <stdio.h>
#include "hashtable.h"
#include "fraudes.h"
#include "arquivo.h"
#include "interface.h"
#include "fatura_pdf.h"


int main(){

    Tabela_hash *tabela = criar_tabela();

    if (tabela==NULL){
        printf("Erro ao alocar a tabela hash!\n");
        return EXIT_FAILURE;
    }

    static char arquivos_lidos[tam_tabela][50];
    int quantidade_arquivos_lidos=0;

    static char numeros_cartoes[tam_tabela][50];
    int quantidade_cartoes=0;

    int user;

    do {
        user=menu();
        switch(user){
            case 0: {
                printf("\nPrograma encerrado!\n\n");
                break;
            }
            case 1: {
                // Se já temos 75 cartões cadastrados, nem adianta ler mais arquivos para cadastrar cartões novos
                if (quantidade_cartoes >= max_cartoes) {
                    printf("\n[ERRO] A memoria ja esta cheia com o limite maximo de %d cartoes!\n\n", max_cartoes);
                    break;
                }

                int qtd_arquivos_antes = quantidade_arquivos_lidos;

                // 1. Lê o arquivo CSV e insere as transações na Tabela Hash
                tabela = ler_arquivo_e_preencher_tabela(tabela, &quantidade_arquivos_lidos, arquivos_lidos);

                // Se o arquivo foi lido com sucesso (a quantidade de arquivos lidos aumentou):
                if (quantidade_arquivos_lidos > qtd_arquivos_antes) {

                    // Reseta o contador para re-mapear os cartões únicos atualizados
                    quantidade_cartoes = 0;

                    // 2. Extrai os cartões únicos respeitando a trava de 75 cartões
                    ler_numeros_cartoes(tabela, numeros_cartoes, &quantidade_cartoes);

                    // 3. Processa apenas os cartões que foram registrados (no máximo 75)
                    criar_arquivos(numeros_cartoes, quantidade_cartoes);
                    ordenar_gavetas_tabela_hash(tabela, numeros_cartoes, quantidade_cartoes);
                    preencher_arquivos(tabela, numeros_cartoes, quantidade_cartoes);

                    printf("\nProcessamento concluido! (%d cartoes registrados no total)\n", quantidade_cartoes);
                }
                break;
            }
            case 2: {
                relatorio_cartao(tabela);
                break;
            }
            case 3: {
                gerar_fatura_pdf(tabela);
                break;
            }
            default: {
                printf("\nOpcao invalida! Digite novamente!\n");
                break;
            }
        }

    } while(user!=0);

    liberar_tabela(tabela);

    return 0;

}
