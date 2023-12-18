/*
    Carolina Elias de Almeida Américo - 13676687
    Agnes Bressan de Almeida - 13677100
*/

#include "../headers/funcionalidades.h"


/* Lê o comando de entrada e chamar a funcionalidade requisitada */
int main() {

    int funcionalidade;
    char arq1[25];
    int n;

    scanf("%d", &funcionalidade);

    switch (funcionalidade) {
    case 8:
        scanf("%s", arq1);
        geraGrafo(arq1);
        break;
    case 9:
        scanf("%s", arq1);
        geraGrafoTransposto(arq1);
        break;
    case 10:
        scanf("%s %d", arq1, &n);
        pesquisaGrafo(arq1, n);
        break;
    case 11:
        scanf("%s", arq1);
        //conexoGrafo(arq1);
        break;     
    case 12:
        scanf("%s", arq1);
        caminhoMaisCurto(arq1);
        break;                   
    default:
        printf("Funcionalidade nao encontrada.");
        break;
    }

    return 0;
}