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

    //Lendo os registros do arquivo de dados
    for(int i = 0; i < 2; i++) {
        if(leRegistroBinario(entrada->p, rAux, cAux) == 0) {
            // printf("\n\nNome tecnologia Origem %d = %s     / tamanho  = %d", i, rAux->nomeTecOrigem.nome, rAux->nomeTecOrigem.tam);
            // printf("\nNome tecnologia Destino %d = %s     / tamanho  = %d\n", i, rAux->nomeTecDestino.nome, rAux->nomeTecDestino.tam);

            // Se nenhuma das tecnologias for nula, adiciona o registro no grafo
            if((rAux->nomeTecOrigem.nome[0] != '$') && (rAux->nomeTecDestino.nome[0] != '$')) {
                adicionaRegistro(rAux, grafo);
            }
            // printf("\nvertice 1 = %s", grafo->primeiroElem->nomeTec);
            // printf("\nvertice 2 = %s", grafo->primeiroElem->proxElem->nomeTec);            
        }
    }

    //printf("\nGRAFO\nPrimeiro vertice = %s", grafo->primeiroElem->nomeTec);
    // printf("\nSegundo vertice = %s", grafo->primeiroElem->proxElem->nomeTec);
    // printf("\nTerceiro vertice = %s", grafo->primeiroElem->proxElem->proxElem->nomeTec);
    // printf("\nAresta = %s", grafo->primeiroElem->proxElem->listaLinear->nomeTecDestino);
    // if(grafo->primeiroElem->proxElem->listaLinear->proxAresta == NULL) {
    //     printf("\nEm teoria era pra entrar aqui\n\n");
    // }    


    // Imprimindo o grafo
    imprimeGrafo(grafo->primeiroElem);

    liberaGrafo(grafo);
    free(cAux);
    free(rAux);
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