#include "interface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int menu(){
    int user;

    printf("\tMENU\n");
    printf("--------------------\n");
    printf("0-Encerrar programa\n1-Carregar arquivo\n2-Relatorio de cartao\n");
    printf("--------------------\n-> ");
    scanf("%d",&user);

    return user;
}
