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

/* Le o registro do endereço do ponteiro do arquivo e armazena no endereço fornecido */
int leRegistroBinario(FILE *p, Registro *r, char *cAux, int RRN) {
    fseek(p, TAM_CABECALHO + TAM_REGISTRO * RRN, SEEK_SET);
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

// Função que busca um nome da tecnologia nos vértices de um grafo
Vertice *buscaVertice(Vertice *v, char *nomeTec) {
    if(v == NULL || strcmp(v->nomeTec, nomeTec) > 0) {
        return NULL;
    } 
    
    // Se o nome das tecnologias for igual, ele retorna o vértice
    if(strcmp(v->nomeTec, nomeTec) == 0) {
        return v;
    }

    if (v->proxElem == NULL) {
        return v;
    }

    Vertice *v2 = buscaVertice(v->proxElem, nomeTec);

    if(v2 == NULL) {
        return v;
    }       
    else {
        return v2;
    }   
}

Aresta *buscaAresta(Aresta *a, char *nomeTec){
    if(a == NULL || strcmp(a->nomeTecDestino, nomeTec) > 0) {
        return NULL;
    }
    
    // Se o nome das tecnologias for igual, ele retorna o vértice
    if(strcmp(a->nomeTecDestino, nomeTec) == 0) {
        return a;
    }

    Aresta *a2 = buscaAresta(a->proxAresta, nomeTec);
    if(a2 == NULL) 
        return a;
    else 
        return a2;
}

Vertice *criaNovoVertice(char *nomeTec, int grupo) {
    // Inicializa um novo vértice e todos os seus atributos
    Vertice *novoVertice = malloc(sizeof(Vertice));
    novoVertice->nomeTec = nomeTec;
    novoVertice->grupo = grupo;
    novoVertice->grauEntrada = 0;
    novoVertice->grauSaida = 0;
    novoVertice->grau = 0;
    novoVertice->listaLinear = NULL;
    novoVertice->proxElem = NULL;

    return novoVertice;
}

/*
    Função recursiva que adiociona um registro no grafo
*/ 
void adicionaRegistro(Registro *r, Grafo *grafo) { 
    
    // Verifica se o grafo está vazio, se estiver, o primeiro elemento será o novo vértice
    if(grafo->primeiroElem == NULL) {
        // Cria o vértice da Tecnologia de Origem
        Vertice *v1 = malloc(sizeof(Vertice));
        v1->nomeTec = malloc(r->nomeTecOrigem.tam * sizeof(char));
        strcpy(v1->nomeTec, r->nomeTecOrigem.nome);
        v1->grupo = r->grupo;
        v1->grauEntrada = 0;
        v1->grauSaida = 1;
        v1->grau = 1;
        v1->listaLinear = malloc(sizeof(Aresta));
        v1->listaLinear->nomeTecDestino = malloc(r->nomeTecDestino.tam * sizeof(char));
        strcpy(v1->listaLinear->nomeTecDestino, r->nomeTecDestino.nome);
        v1->listaLinear->peso = r->peso;
        v1->listaLinear->proxAresta = NULL;

        // Cria o vértice da tecnologia de destino
        Vertice *v2 = malloc(sizeof(Vertice));  
        v2->nomeTec = malloc(r->nomeTecDestino.tam * sizeof(char));
        strcpy(v2->nomeTec, r->nomeTecDestino.nome);
        v2->grupo = r->grupo;
        v2->grauEntrada = 1;
        v2->grauSaida = 0;
        v2->grau = 1;
        v2->listaLinear = NULL;
        
        // Se os nomes das tecnologias forem iguais
        if(strcmp(r->nomeTecOrigem.nome, r->nomeTecDestino.nome) == 0) {
            free(v2);   // libera o vértice da tecnologia de destino (já que é a mesma da de origem)

            // Adiciona a tecnologia de origem como primeiro vértice do grafo
            v1->grauEntrada++;
            v1->grau++;
            grafo->primeiroElem = v1;
            grafo->numVertices++;
            grafo->numArestas++;
        }

        // Se o nome da tecnologia origem for menor 
        else if(strcmp(r->nomeTecOrigem.nome, r->nomeTecDestino.nome) < 0) {           
            v1->proxElem = v2;          // v1 aponta para v2
            v2->proxElem = NULL;
            grafo->primeiroElem = v1;   // v1 é o primeiro elemento do grafo
            grafo->numVertices += 2;
            grafo->numArestas++;
        }

        // Se o nome da tecnologia de destino for menor
        else if(strcmp(r->nomeTecOrigem.nome, r->nomeTecDestino.nome) > 0) { 
            v2->proxElem = v1;          // v2 aponta para v1
            v1->proxElem = NULL;
            grafo->primeiroElem = v2;   // v2 é o primeiro elemento do grafo
            grafo->numVertices += 2;
            grafo->numArestas++;
        }

        return;
    } 
    
    // Se o grafo não estiver vazio, busca se já existem vértices com os nomes da tecnologia de origem
    Vertice *vAux = buscaVertice(grafo->primeiroElem, r->nomeTecOrigem.nome);
    
    // Se o vértice não existe e se o nome da tecnologia for o primeiro do grafo
    if(vAux == NULL) {
        Vertice *v1 = malloc(sizeof(Vertice));
        v1->nomeTec = malloc(r->nomeTecOrigem.tam * sizeof(char));
        strcpy(v1->nomeTec, r->nomeTecOrigem.nome);
        v1->grau = 1;
        v1->grauEntrada = 0;
        v1->grauSaida = 1;
        v1->grupo = r->grupo;
        v1->listaLinear = malloc(sizeof(Aresta));
        v1->listaLinear->nomeTecDestino = malloc(r->nomeTecDestino.tam * sizeof(char));
        strcpy(v1->listaLinear->nomeTecDestino, r->nomeTecDestino.nome);
        v1->listaLinear->peso = r->peso;
        v1->listaLinear->proxAresta = NULL;

        // O próximo elemento depois de v1 vai ser o vértice que até agora era o 1°
        v1->proxElem = grafo->primeiroElem;

        // O primeiro vértice do grafo passa a ser v1
        grafo->primeiroElem = v1;

        grafo->numArestas++;
        grafo->numVertices++;
    }

    // Se o vértice já existir
    else if(strcmp(r->nomeTecOrigem.nome, vAux->nomeTec) == 0) { 
        vAux->grupo = r->grupo;

        // Se o vértice ainda não tiver nenhuma aresta saindo dele
        if(vAux->listaLinear == NULL) {
            vAux->listaLinear = malloc(sizeof(Aresta));
            vAux->listaLinear->nomeTecDestino = malloc(r->nomeTecDestino.tam * sizeof(char));
            strcpy( vAux->listaLinear->nomeTecDestino, r->nomeTecDestino.nome);
            vAux->listaLinear->peso = r->peso;
            vAux->listaLinear->proxAresta = NULL;

            vAux->grau++;
            vAux->grauSaida++;
            grafo->numArestas++;
        }

        // Se já tiver, verifica se já existe uma aresta para a tec de destino
        else {
            Aresta *a = buscaAresta(vAux->listaLinear, r->nomeTecDestino.nome);

            // Se já existe uma aresta saindo da tec Origem e indo para a de Destino, ele retorna
            if(strcmp(a->nomeTecDestino, r->nomeTecDestino.nome) == 0) {
                return; 
            }

            // Se não, ele cria e adiciona uma nova aresta
            Aresta *novaAresta = malloc(sizeof(Aresta));
            novaAresta->nomeTecDestino = malloc(r->nomeTecDestino.tam * sizeof(char));
            strcpy(novaAresta->nomeTecDestino, r->nomeTecDestino.nome);
            novaAresta->peso = r->peso;

            // Adicionando nova aresta na lista linear
            novaAresta->proxAresta = a->proxAresta;
            a->proxAresta = novaAresta;

            vAux->grau++;
            vAux->grauSaida++;
            grafo->numArestas++;
        }

    }
    
    // se ainda não tem um vértice da tecnologia de origem do registro
    else {  
        Vertice *v1 = malloc(sizeof(Vertice));
        v1->nomeTec = malloc(r->nomeTecOrigem.tam * sizeof(char));
        strcpy(v1->nomeTec, r->nomeTecOrigem.nome);
        v1->grau = 1;
        v1->grauEntrada = 0;
        v1->grauSaida = 1;
        v1->grupo = r->grupo;
        v1->listaLinear = malloc(sizeof(Aresta));
        v1->listaLinear->nomeTecDestino = malloc(r->nomeTecDestino.tam * sizeof(char));
        strcpy(v1->listaLinear->nomeTecDestino, r->nomeTecDestino.nome);
        v1->listaLinear->peso = r->peso;
        v1->listaLinear->proxAresta = NULL;

        // O próximo elemento depois de v1
        v1->proxElem = vAux->proxElem;

        // O elemento antes de v1 aponta para ele
        vAux->proxElem = v1;

        grafo->numArestas++;
        grafo->numVertices++;
    }

    // Agora busca pela tecnologia de destino
    vAux = buscaVertice(grafo->primeiroElem, r->nomeTecDestino.nome);
    
    // Se o vértice não existe e se o nome da tecnologia for o primeiro do grafo
    if(vAux == NULL) {
        Vertice *v2 = malloc(sizeof(Vertice));
        v2->nomeTec = malloc(r->nomeTecDestino.tam * sizeof(char));
        strcpy(v2->nomeTec, r->nomeTecDestino.nome);
        v2->grau = 1;
        v2->grauEntrada = 1;
        v2->grauSaida = 0;
        v2->grupo = r->grupo;
        v2->listaLinear = NULL;

        // O próximo elemento depois de v2 vai ser o vértice que até agora era o 1°
        v2->proxElem = grafo->primeiroElem;

        // O primeiro vértice do grafo passa a ser v2
        grafo->primeiroElem = v2;

        grafo->numVertices++;
    }

    // Se o vértice já existir
    else if(strcmp(r->nomeTecDestino.nome, vAux->nomeTec) == 0) { 
        vAux->grau++;
        vAux->grauEntrada++;
    }
    
    // se ainda não tem um vértice da tecnologia de destino do registro
    else {  
        Vertice *v2 = malloc(sizeof(Vertice));
        v2->nomeTec = malloc(r->nomeTecDestino.tam * sizeof(char));
        strcpy(v2->nomeTec, r->nomeTecDestino.nome);
        v2->grau = 1;
        v2->grauEntrada = 1;
        v2->grauSaida = 0;
        v2->grupo = r->grupo;
        v2->listaLinear = NULL;

        // O próximo elemento depois de v2
        v2->proxElem = vAux->proxElem;

        // O elemento antes de v2 aponta para ele
        vAux->proxElem = v2;

        grafo->numVertices++;
    }
}

void imprimeArestas(Vertice *v, Aresta *a) {
    if(a == NULL) {
        return;
    }

    printf("%s %d %d %d %d ", v->nomeTec, v->grupo, v->grauEntrada, v->grauSaida, v->grau);
    printf("%s %d\n", a->nomeTecDestino, a->peso);

    if(a->proxAresta == NULL)
        return;
    imprimeArestas(v, a->proxAresta);
    return;
}

void imprimeGrafo(Vertice *v) {
    if(v == NULL) {
        return; 
    }


    // roda a lista de arestas do vértice
    //Aresta *a = v->listaLinear;
    imprimeArestas(v, v->listaLinear);

    if(v->proxElem == NULL)
        return;
    
    imprimeGrafo(v->proxElem);
}

void liberaArestas(Aresta *a) {
    if(a->proxAresta != NULL) {
        liberaArestas(a->proxAresta);
    } 

    free(a->nomeTecDestino);
    free(a);
    printf("\nlibera aresta");
}

void liberaVertices(Vertice *v) {
    if(v->listaLinear != NULL) {
        liberaArestas(v->listaLinear);
    }
    if(v->proxElem != NULL) {
        liberaVertices(v->proxElem);
    }

    free(v->nomeTec);
    free(v);
    printf("\nlibera vertice");
}

void liberaGrafo(Grafo *grafo) {
    if(grafo->primeiroElem != NULL) {
        liberaVertices(grafo->primeiroElem);
    }

    printf("\naqui");

    free(grafo);
}

