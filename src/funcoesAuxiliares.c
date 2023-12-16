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
    
    // Verifica se o grafo foi criado
    // if(grafo->numVertices == 0) {
    //     printf("Falha na execucao da funcionalidade.");
    //     return;
    // }

    // Inicializa um novo vértice e todos os seus atributos
    Vertice *novoVertice = malloc(sizeof(Vertice));
    novoVertice->nomeTecOrigem = r->nomeTecOrigem.nome;
    novoVertice->grupo = r->grupo;
    novoVertice->grauEntrada = 0;
    novoVertice->grauSaida = 0;
    novoVertice->grau = 0;
    novoVertice->listaLinear = NULL;
    novoVertice->proxElem = 0;

    // Adiciona o vértice no grafo
    //verifica se o grafo está vazio, se estiver o primeiro elemento será o novo vértice
    if(grafo->primeiroElem == NULL) {
        grafo->primeiroElem = novoVertice;
    } else { // se não estiver vazio, o novo vértice será adicionado no final da lista
        //cria um vertice auxiliar
        Vertice *aux = grafo->primeiroElem;
        // percorre toda a lista de vértices do grafo até encontrar o último vértice
        while(aux->proxElem != NULL) {
            aux = aux->proxElem;
        }
        // adiciona o novo vértice no final da lista
        aux->proxElem = novoVertice;
    }
    // incrementa o número de vértices do grafo
    grafo->numVertices++;

    // Aloca uma nova aresta
    // aresta que liga o vértice de origem ao vértice de destino
    Aresta *novaAresta = malloc(sizeof(Aresta));
    novaAresta->nomeTecDestino = r->nomeTecDestino.nome;
    novaAresta->peso = r->peso;
    novaAresta->proxAresta = NULL;

    // Adiciona a aresta na lista linear do vértice
    //verifica se o vértice não possui nenhuma aresta, se não possuir a primeira aresta será a nova aresta
    if(novoVertice->listaLinear == NULL) {
        novoVertice->listaLinear = novaAresta;
    } else {
        // se possuir, a nova aresta será adicionada no final da lista
        Aresta *aux = novoVertice->listaLinear;
        // percorre toda a lista de arestas do vértice até encontrar a última aresta
        while(aux->proxAresta != NULL) {
            aux = aux->proxAresta;
        }
        aux->proxAresta = novaAresta;
    }
    novoVertice->grauSaida++;
    novoVertice->grau++;
    grafo->numArestas++;
}

void imprimeGrafo(Grafo *grafo) {

    Vertice *v = grafo->primeiroElem;
    Aresta *a;

    while(v != NULL) {
        a = v->listaLinear;

        printf("%s %d %d %d %d ", v->nomeTecOrigem, v->grupo, v->grauEntrada, v->grauSaida, v->grau);


        // roda a lista de arestas do vértice
        while(a != NULL) {
            printf("%s %d\n", a->nomeTecDestino, a->peso);
            a = a->proxAresta;
        }

        v = v->proxElem;
    }
}