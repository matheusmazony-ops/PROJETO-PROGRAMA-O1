#include "fatura_pdf.h"
#include "pdfgen.h"
#include "fraudes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static const char *categorias_nomes[] = {
    "","Entretenimento", "Alimentacao", "Combustivel/Transporte", "Supermercado Online", "Supermercado Presencial", "Saude e Fitness", "Casa", "Criancas e Pets",
    "Itens Variados Online", "Itens Variados Presencial", "Cuidados Pessoais", "Compras Online", "Compras Presenciais", "Viagem"
};


void desenhar_cabecalho_tabela(struct pdf_doc *pdf, float y) {
    /* Os parâmetros da função consistem em: ponteiro do documento PDF; ponteiro da página apontada; posição horizontal;
    posição vertical; largura do retângulo; altura do retângulo; espessura da linha de contorno do retângulo;
    cor do interior do retângulo; cor da borda do contorno do retângulo */
    pdf_add_filled_rectangle(pdf, NULL, 40, y - 5, 515, 20, 0, PDF_RGB(230, 230, 230), PDF_TRANSPARENT);

    pdf_set_font(pdf, "Times-Roman-Bold");
    /* Os parâmetros da função consistem em: Ponteiro do documento PDF; ponteiro da página; texto em string para printar;
    tamanho da fonte; posição x e y na página; cor do texto */
    pdf_add_text(pdf, NULL, "Data / Hora (UTC)", 10, 45, y, PDF_BLACK);
    pdf_add_text(pdf, NULL, "Categoria", 10, 220, y, PDF_BLACK);
    pdf_add_text(pdf, NULL, "Valor (R$)", 10, 350, y, PDF_BLACK);
    pdf_add_text(pdf, NULL, "Status", 10, 480, y, PDF_BLACK);
    // A função pdf_set_font foi colocado duas vezes para diferenciar entre o cabeçalho e o corpo do texto abaixo dele
    pdf_set_font(pdf, "Times-Roman");
}


void gerar_fatura_pdf(Tabela_hash *tabela){
    (void) tabela;

    char numero_cartao[32];
    int mes_desejado, ano_desejado;

    printf("\n=== GERAR FATURA EM PDF ===\n");
    printf("Digite o numero do cartao:");
    scanf("%31s", numero_cartao);
    printf("Digite o mes (1 - 12): ");
    scanf("%d", &mes_desejado);
    printf("Digite o ano (ex: 2019): ");
    scanf("%d", &ano_desejado);

    char nome_csv[60];
    snprintf(nome_csv, sizeof(nome_csv), "%s.csv", numero_cartao);

    FILE *arquivo = fopen(nome_csv, "r");
    if(arquivo == NULL){
        printf("\nO arquivo '%s' nao foi encontrado! Execute a Opcao 1 do menu primeiro.\n\n", nome_csv);
        return;
    }

    char linha[256];
    int total_transacoes = 0;
    double total_fatura = 0.0;

    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        int ano, mes, dia, hora, minuto, segundo, categoria;
        float valor;
        char fraude_a, fraude_b, fraude_c;

        if(sscanf(linha, "%d-%d-%d %d:%d:%d (UTC) %d,%f,%c,%c,%c",
                  &ano, &mes, &dia, &hora, &minuto, &segundo,
                  &categoria, &valor, &fraude_a, &fraude_b, &fraude_c) == 11){

            if(mes == mes_desejado && ano == ano_desejado){
                total_transacoes++;
                total_fatura += valor;
            }
        }
    }

    if(total_transacoes == 0){
        printf("\nNenhuma transacao encontrada para o cartao %s no periodo %02d/%d.\n\n",
               numero_cartao, mes_desejado, ano_desejado);
        fclose(arquivo);
        return;
    }


    struct pdf_doc *pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, NULL);
    if(!pdf){
        printf("\nFalha ao criar o documento PDF.\n");
        return;
    }

    pdf_append_page(pdf);

    pdf_add_filled_rectangle(pdf, NULL, 0, 770, PDF_A4_WIDTH, 72, 0, PDF_RGB(24, 76, 120), PDF_TRANSPARENT);
    pdf_set_font(pdf, "Times-Roman-Bold");
    pdf_add_text(pdf, NULL, "BANCO DO BRASIL - FATURA DO CARTAO", 18, 40, 800, PDF_WHITE);
    pdf_set_font(pdf, "Times-Roman");
    pdf_add_text(pdf, NULL, "Relatorio consolidado de Transacoes", 10, 40, 782, PDF_WHITE);

    // Informações da transação
    char buffer[128];
    pdf_set_font(pdf, "Times-Roman-Bold");
    pdf_add_text(pdf, NULL, "DETALHES DA FATURA", 12, 40, 745, PDF_RGB(24, 76, 120));
    pdf_set_font(pdf, "Times-Roman");

    // A função snprintf tem o papel de formatar e gravar os dados em uma string, nesse caso, a string sobre o número do cartão
    snprintf(buffer, sizeof(buffer), "Numero do cartao: %s", numero_cartao);
    pdf_add_text(pdf, NULL, buffer, 10, 40, 728, PDF_BLACK);

    snprintf(buffer, sizeof(buffer), "Periodo de referencia: %d/%d", mes_desejado, ano_desejado);
    pdf_add_text(pdf, NULL, buffer, 10, 40, 713, PDF_BLACK);

    snprintf(buffer, sizeof(buffer), "Qtd. de Compras: %d", total_transacoes);
    pdf_add_text(pdf, NULL, buffer, 10, 330, 728, PDF_BLACK);

    snprintf(buffer, sizeof(buffer), "Valor total: R$ %.2f", total_fatura);
    pdf_set_font(pdf, "Times-Roman-Bold");
    pdf_add_text(pdf, NULL, buffer, 11, 330, 713, PDF_RGB(180, 40, 40));
    pdf_set_font(pdf, "Times-Roman");

    // Adiciona uma linha divisora
    pdf_add_line(pdf, NULL, 40, 698, 555, 698, 1, PDF_RGB(200, 200, 200));

    // Tabela de transação
    float y_pos = 675;
    desenhar_cabecalho_tabela(pdf, y_pos);
    y_pos -= 20;

    rewind(arquivo);

    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        int ano, mes, dia, hora, minuto, segundo, categoria;
        float valor;
        char fraude_a, fraude_b, fraude_c;

        if(sscanf(linha, "%d-%d-%d %d:%d:%d (UTC) %d,%f,%c,%c,%c",
                  &ano, &mes, &dia, &hora, &minuto, &segundo, &categoria, &valor,
                  &fraude_a, &fraude_b, &fraude_c) == 11){

            if(mes == mes_desejado && ano == ano_desejado){
                // Cria uma nova página se Y ficar muito baixo
                if(y_pos < 60){
                    pdf_append_page(pdf);
                    y_pos = 800;
                    desenhar_cabecalho_tabela(pdf, y_pos);
                    y_pos -= 20;
                }

                // Formata e grava os dados da data e hora
                snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d", ano, mes, dia, hora, minuto);
                pdf_add_text(pdf, NULL, buffer, 9, 45, y_pos, PDF_BLACK);

                // Identifica a categoria numérica da transação
                const char *cat_nome = (categoria >= 1 && categoria <= 14) ? categorias_nomes[categoria] : "Outros";
                pdf_add_text(pdf, NULL, cat_nome, 9, 220, y_pos, PDF_BLACK);

                // Formatando a quantidade das transações
                snprintf(buffer, sizeof(buffer), "R$ %.2f", valor);
                pdf_add_text(pdf, NULL, buffer, 9, 350, y_pos, PDF_BLACK);

                if(fraude_a == 'Y' || fraude_b == 'Y' || fraude_c == 'Y'){
                    pdf_set_font(pdf, "Times-Roman-Bold");
                    pdf_add_text(pdf, NULL, "SUSPEITA", 9, 480, y_pos, PDF_RGB(200, 30, 30));
                    pdf_set_font(pdf, "Times-Roman");
                } else{
                    pdf_add_text(pdf, NULL, "APROVADA", 9, 480, y_pos, PDF_RGB(30, 140, 30));
                }

                pdf_add_line(pdf, NULL, 40, y_pos - 4, 555, y_pos - 4, 0.5f, PDF_RGB(240, 240, 240));
                y_pos -= 16;
            }
        }
    }

    fclose(arquivo);

    // Salvar o PDF
    char nome_pdf[64];
    snprintf(nome_pdf, sizeof(nome_pdf), "fatura_%s_%02d_%d.pdf", numero_cartao, mes_desejado, ano_desejado);

    if(pdf_save(pdf, nome_pdf) >= 0){
        printf("\nFatura gerada com sucesso: '%s'\n\n", nome_pdf);
    } else{
        printf("\nFalha ao salvar o arquivo PDF.\n\n");
    }

    pdf_destroy(pdf);
}
