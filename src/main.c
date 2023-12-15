/*
    Carolina Elias de Almeida Américo - 13676687
    Agnes Bressan de Almeida - 13677100
*/

#include "../headers/funcionalidades.h"


/* Lê o comando de entrada e chamar a funcionalidade requisitada */
int main() {

    int funcionalidade;
    char arq1[25];

    scanf("%d", &funcionalidade);

    switch (funcionalidade) {
    case 8:
        scanf("%s", arq1);
        printf("Funcionalidade ainda nao implementada\n");
        geraGrafo(arq1);
        break;
    case 9:
        printf("Funcionalidade ainda nao implementada\n");
        break;
    case 10:
        printf("Funcionalidade ainda nao implementada\n");
        break;
    case 11:
        printf("Funcionalidade ainda nao implementada\n");
        break;       
    case 12:
        printf("Funcionalidade ainda nao implementada\n");
        break;                 
    default:
        printf("Funcionalidade nao encontrada.");
        break;
    }

    return 0;
}