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
    for(int i = 0; i < entrada->rC.proxRRN; i++) {
        if(leRegistroBinario(entrada->p, rAux, cAux, i) == 0) {
            // Se nenhuma das tecnologias for nula, adiciona o registro no grafo
            if((rAux->nomeTecOrigem.tam != 0 && rAux->nomeTecDestino.tam != 0) && (rAux->nomeTecOrigem.nome[0] != '$' && rAux->nomeTecDestino.nome[0] != '$')) {
                adicionaRegistro(rAux, grafo);
            }
        }
    }  


    // Imprimindo o grafo
    imprimeGrafo(grafo->primeiroElem);

    liberaGrafo(grafo); // desaloca todas as estruturas do grafo (ele mesmo, seus vértices, arestas, ...)
    free(cAux);
    free(rAux);
    free(entrada);
    fclose(p);  // fecha o arquivo binário
}

//funcionalidade 9
void geraGrafoTransposto(char *arq1) {
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
        if(leRegistroBinario(entrada->p, rAux, cAux, i) == 0) {
            // Se nenhuma das tecnologias for nula, adiciona o registro no grafo
            if((rAux->nomeTecOrigem.tam != 0 && rAux->nomeTecDestino.tam != 0) && (rAux->nomeTecOrigem.nome[0] != '$' && rAux->nomeTecDestino.nome[0] != '$')) {
                adicionaRegistroTransposto(rAux, grafo);
            }
        }
    }  

    // Imprimindo o grafo
    imprimeGrafo(grafo->primeiroElem);

    liberaGrafo(grafo); // desaloca todas as estruturas do grafo (ele mesmo, seus vértices, arestas, ...)
    free(cAux);
    free(rAux);
    free(entrada);
    fclose(p);  // fecha o arquivo binário
}

//funcionalidade 10
void pesquisaGrafo(char *arq1, int n){

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

    Grafo *grafo = criaGrafo();

    //Lendo os registros do arquivo de dados
    for(int i = 0; i < entrada->rC.proxRRN; i++) {
        if(leRegistroBinario(entrada->p, rAux, cAux, i) == 0) {
            // Se nenhuma das tecnologias for nula, adiciona o registro no grafo
            if((rAux->nomeTecOrigem.tam != 0 && rAux->nomeTecDestino.tam != 0) && (rAux->nomeTecOrigem.nome[0] != '$' && rAux->nomeTecDestino.nome[0] != '$')) {
                adicionaRegistro(rAux, grafo);
            }
        }
    }
    char *valorCampo = malloc(sizeof(char));

    for(int i = 0; i < n; i++) {
        scanf("%c", valorCampo);
        scan_quote_string(valorCampo);

        search(grafo, valorCampo);                
    }


    liberaGrafo(grafo); // desaloca todas as estruturas do grafo (ele mesmo, seus vértices, arestas, ...)
    free(cAux);
    free(valorCampo);
    free(rAux);
    free(entrada);
    fclose(p);  // fecha o arquivo binário

}

//funcionalidade 11
void conexoGrafo(char *arq1){

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

    Grafo *grafo = criaGrafo();

    //Lendo os registros do arquivo de dados
    for(int i = 0; i < entrada->rC.proxRRN; i++) {
        if(leRegistroBinario(entrada->p, rAux, cAux, i) == 0) {
            // Se nenhuma das tecnologias for nula, adiciona o registro no grafo
            if((rAux->nomeTecOrigem.tam != 0 && rAux->nomeTecDestino.tam != 0) && (rAux->nomeTecOrigem.nome[0] != '$' && rAux->nomeTecDestino.nome[0] != '$')) {
                adicionaRegistro(rAux, grafo);
            }
        }
    }

    // função para verificar se o grafo é fortemente conexo
    int aux = verificaConexo(grafo);

    if(aux == 1){
        printf("Sim, o grafo e fortemente conexo e possui %d componente.\n", aux);
    }else{
        printf("Nao, o grafo nao e fortemente conexo e possui %d componentes.\n", aux);
    }

    liberaGrafo(grafo); // desaloca todas as estruturas do grafo (ele mesmo, seus vértices, arestas, ...)
    free(cAux);
    free(rAux);
    free(entrada);
    fclose(p);  // fecha o arquivo binário
}