/*
    Carolina Elias de Almeida Américo - 13676687
    Agnes Bressan de Almeida - 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_REGISTRO 76  //tamanho dos registros
#define TAM_CAMPOS_FIXO 21  //tamanho total dos campos de tamanho fixo
#define TAM_CABECALHO 13 //tamanho do registro de cabeçalho
#define OFFSET_TAM_ORIGEM(RRN) TAM_CABECALHO + 76*RRN + 13
#define INFINITO 2147483647


// struct do registro de cabeçalho do arquivo de dados
typedef struct { 
    char status; 
    int proxRRN;
    int nTec; 
    int nPares; 
} RegCabecalho; //tamanho fixo de 13 bytes

typedef struct {
    FILE* p;
    RegCabecalho rC;
} ArquivoDados;

typedef struct { //struct voltada a variáveis String com tamanho variável
    int tam;
    char nome[(TAM_REGISTRO-TAM_CAMPOS_FIXO)/2];  
    /* no tamanho do vetor calcula o espaço que tem para variáveis com tamanho variável, pegando o 
    tamanho total do registro e subtraindo pelo tamanho ocupado pelas variáveis com tamanho fixo */
} StringVariavel;

typedef struct {
    char removido;
    int grupo;
    int popularidade;
    int peso;
    StringVariavel nomeTecOrigem;
    StringVariavel nomeTecDestino;   
} Registro;

typedef struct aresta{
    char *nomeTecDestino;
    int peso;
    struct aresta *proxAresta;
} Aresta;

typedef struct elemento {
    char *nomeTec;
    int grupo;
    int grauEntrada;
    int grauSaida;
    int grau;
    Aresta *listaLinear;
    struct elemento *proxElem;
} Vertice;

typedef struct {
    int numVertices;
    int numArestas;
    Vertice *primeiroElem;
} Grafo;

typedef struct{
    int top;
    int* array;
} Stack;

// struct usado no algoritmo de dijkstra
typedef struct {
    Vertice v;
    int distancia;
    int visitado;
} estruturaAux;


/***************************************************************************************************
 *                                Funções do trabalho introdutório
****************************************************************************************************/

void leRegistroCabecalho(ArquivoDados *arq);

int leRegistroBinario(FILE *p, Registro *r, char *cAux, int RRN);



/***************************************************************************************************
 *                                Funções próprias do trabalho 2
****************************************************************************************************/

Grafo *criaGrafo();

Vertice *buscaVertice(Vertice *v, char *nomeTec);

// Função que adiociona um registro no grafo
void adicionaRegistro(Registro *r, Grafo *grafo);

// Função que adiociona um registro no grafo transposto
void adicionaRegistroTransposto(Registro *r, Grafo *grafo);

void imprimeGrafo(Vertice *v);

void liberaGrafo(Grafo *grafo);

void imprimeTecAresta(Aresta *a);

void preenche_ordem(Grafo* grafo, Stack* stack, int passou[], int i);

void DFS(Grafo* grafo, int passou[], int i);

Vertice addVerticeOrigem(char *nomeTecnologia, int grupo);

int encontrarOuCriarVertice(Grafo *grafo, char *nomeTecnologia);

void adicionarAresta(Grafo *grafo, int verticeOrigem, char *nomeTecnologiaDestino, int peso, int grupo);

void DFS(Grafo* grafo,int passou[], int v);

int verificaConexo(Grafo *grafo);

// descobre a menor distância entre dois vértices de um grafo
int algoritmoDijkstra(Grafo *grafo, char *tecOrigem, char *tecDestino);

