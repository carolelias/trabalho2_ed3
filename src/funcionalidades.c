#include "../headers/funcoesAuxiliares.h"
#include "../headers/funcoesFornecidas.h"

//funcionalidade 8
void geraGrafo(char *arq1) {
    // Abrindo o arquivo de dados
    FILE *p = fopen(arq1, "r");
    if(p == NULL){
        printf("Falha na execucao da funcionalidade.");
        return;
    }

    // Alocamos espaço para um struct do arquivo de dados
    ArquivoDados *entrada = malloc(sizeof(ArquivoDados));

    // Definindo o ponteiro para o arquivo de entrada 
    entrada->p = p;
    leRegistroCabecalho(entrada);

    Registro *rAux = malloc(sizeof(Registro));
    char *cAux = malloc(sizeof(char));

    // Criando o grafo
    Grafo *grafo = criaGrafo();

    // printf("prox rrn do arquivo de dados  = %d", entrada->rC.proxRRN);

    //Lendo os registros do arquivo de dados
    for(int i = 0; i < entrada->rC.proxRRN; i++) {
        if(leRegistroBinario(entrada->p, rAux, cAux) == 0) {
            // printf("\nNome tecnologia Origem %d = %s", i, rAux->nomeTecOrigem.nome);
            //adiciona o registro no grafo
            adicionaRegistro(rAux, grafo);
            // Imprimindo o grafo
            imprimeGrafo(grafo);
        }
    }


    free(cAux);
    free(rAux);
    free(grafo);
    free(entrada);
    fclose(p);
}

//funcionalidade 10
void pesquisaGrafo(char *arq1){
    // Abrindo o arquivo de dados
    FILE *p = fopen(arq1, "r");
    if(p == NULL){
        printf("Falha na execucao da funcionalidade.");
        return;
    }

    // Alocamos espaço para um struct do arquivo de dados
    ArquivoDados *entrada = malloc(sizeof(ArquivoDados));

    // Definindo o ponteiro para o arquivo de entrada 
    entrada->p = p;
    leRegistroCabecalho(entrada);

    Registro *rAux = malloc(sizeof(Registro));
    char *cAux = malloc(sizeof(char));

    // Criando o grafo
    Grafo *grafo = criaGrafo();

    //Lendo os registros do arquivo de dados
    for(int i = 0; i < entrada->rC.proxRRN; i++) {
        if(leRegistroBinario(entrada->p, rAux, cAux) == 0) {
            // printf("\nNome tecnologia Origem %d = %s", i, rAux->nomeTecOrigem.nome);
            //adiciona o registro no grafo
            adicionaRegistro(rAux, grafo);
            // Imprimindo o grafo
            // imprimeGrafo(grafo);
        }
    }

    Vertice *v = grafo->primeiroElem;
    //Aresta *a;

    while(v != NULL) {
        //a = v->listaLinear;

        v = v->proxElem;
    }
    

}