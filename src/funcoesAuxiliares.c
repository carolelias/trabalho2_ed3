#include "../headers/funcoesAuxiliares.h"

// Função que lê o registro de cabeçalho do arquivo de dados
void leRegistroCabecalho(ArquivoDados *arq) {
    fread(&arq->rC.status, 1, 1, arq->p);
    fread(&arq->rC.proxRRN, sizeof(int), 1, arq->p);
    fread(&arq->rC.nTec, sizeof(int), 1, arq->p);
    fread(&arq->rC.nPares, sizeof(int), 1, arq->p);

    // verifica se o status é diferente de 0 (se o arquivo está estável)
    if(arq->rC.status == '0') {
        printf("Falha na execucao da funcionalidade.");
        exit(0);
    }
}

/* Le o registro do endereço do ponteiro do arquivo e armazena do endereço fornecido */
int leRegistroBinario(FILE *p, Registro *r, char *cAux) {
    
    fread(&r->removido, 1, 1, p);
    if(r->removido == '1') {
        fseek(p, TAM_REGISTRO-1, SEEK_CUR);
        return 1;
    }
        
    fread(&r->grupo, sizeof(int), 1, p);
    fread(&r->popularidade, sizeof(int), 1, p);
    fread(&r->peso, sizeof(int), 1, p);
    fread(&r->nomeTecOrigem.tam, sizeof(int), 1, p);
    for(int i = 0; i < r->nomeTecOrigem.tam; i++) {
        fread(cAux, 1, 1, p);
        r->nomeTecOrigem.nome[i] = *cAux;
    }
    r->nomeTecOrigem.nome[r->nomeTecOrigem.tam] = '\0';     // indica o fim da string
    fread(&r->nomeTecDestino.tam, sizeof(int), 1, p);
    for(int i = 0; i < r->nomeTecDestino.tam; i++) {
        fread(cAux, 1, 1, p);
        r->nomeTecDestino.nome[i] = *cAux;
    }
    r->nomeTecDestino.nome[r->nomeTecDestino.tam] = '\0';   // indica o fim da string

    // lendo o lixo
    for(int i = 0; i < TAM_REGISTRO - (r->nomeTecOrigem.tam + r->nomeTecDestino.tam + TAM_CAMPOS_FIXO); i++) {
        fread(cAux, 1, 1, p);
    }

    return 0;
}

Grafo *criaGrafo() {
    // Aloca o grafo e inicializa seus atributos 
    Grafo *grafo = malloc(sizeof(grafo));
    grafo->numVertices = 0;     // ainda não possui vértices
    grafo->numArestas = 0;      // ainda não possui arestas
    grafo->primeiroElem = NULL; // grafo vazio

    return grafo;
}


/*
    Função recursiva que adiociona um registro no grafo
*/ 
void adicionaRegistro(Registro *r, Grafo *grafo, Vertice *v) {
    if(grafo->numVertices == NULL) {

    }
}