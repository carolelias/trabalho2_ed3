#include "../headers/funcoesAuxiliares.h"

/***************************************************************************************************
 *                                Funções do trabalho introdutório
****************************************************************************************************/

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





/***************************************************************************************************
 *                                Funções próprias do trabalho 2
****************************************************************************************************/

// Função que aloca e inicializa um grafo
Grafo *criaGrafo() {
    // Aloca o grafo e inicializa seus atributos 
    Grafo *grafo = malloc(sizeof(grafo));
    grafo->numVertices = 0;     // ainda não possui vértices
    grafo->numArestas = 0;      // ainda não possui arestas
    grafo->primeiroElem = NULL; // grafo vazio

    return grafo;
}


/*
    Função recursiva que busca um nome da tecnologia nos vértices de um grafo.
    Retorna o vértice no qual a tecnologia foi encontrada ou o vértice que irá anteceder a 
    tecnologia que está sendo buscada
*/ 
Vertice *buscaVertice(Vertice *v, char *nomeTec) {
    // Se o nome da tecnologia do vértice for maior que a tecnologia buscada
    if(strcmp(v->nomeTec, nomeTec) > 0) {
        return NULL;    // retorna que não achou o vértice da tecnologia
    } 
    
    // Se o nome das tecnologias for igual, ele retorna o vértice 
    if(strcmp(v->nomeTec, nomeTec) == 0) {
        return v;
    }

    /* Se chegou no fim dos vértices retorna o vértice atual ("último vértice") 
       O vértice da tecnologia buscada será criado e adicionado depois do último */
    if (v->proxElem == NULL) {
        return v;
    }

    // Chamada recursiva (busca a tecnologia nos próximos vértices)
    Vertice *v2 = buscaVertice(v->proxElem, nomeTec);

    // Se o retorno da chamada recursiva for nulo, retorna o vértice atual
    if(v2 == NULL) {
        return v;
    }       

    // Senão, retorna o que veio da recursão
    else {
        return v2;
    }   
}

/*
    Função recursiva que busca uma aresta numa lista linear de arestas.
    Retorna a aresta no qual a tecnologia foi encontrada ou a aresta que irá anteceder a 
    tecnologia que está sendo buscada
*/ 
Aresta *buscaAresta(Aresta *a, char *nomeTec) {
    // Se o nome da tecnologia da aresta for maior que a tecnologia buscada
    if(strcmp(a->nomeTecDestino, nomeTec) > 0) {
        return NULL;    // retorna que não encontrou a aresta
    }
    
    // Se o nome das tecnologias for igual, ele retorna a aresta da tecnologia
    if(strcmp(a->nomeTecDestino, nomeTec) == 0) {
        return a;
    }

    // Se ele não encontra a tecnologia, ele retorna a aresta que irá anteceder a nova aresta
    if(a->proxAresta == NULL) {
        return a;
    }  

    // Recursão (procura a tecnologia na próxima aresta)
    Aresta *a2 = buscaAresta(a->proxAresta, nomeTec);

    /* Se ele não achou a tecnologia na aresta seguinte, retorna a aresta atual.   
       A aresta atual irá anteceder a nova aresta que será criada com a tecnologia que não foi achada*/
    if(a2 == NULL) 
        return a;

    // Se a2 não for nulo, retorna ela (que veio das chamadas recursivas)
    else 
        return a2; 
}

/*
    Função que adiociona um registro no grafo
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

            // Se a aresta for ser a primeira da lista linear de arestas:
            if(a == NULL) {
                Aresta *novaAresta = malloc(sizeof(Aresta));
                novaAresta->nomeTecDestino = malloc(r->nomeTecDestino.tam * sizeof(char));
                strcpy(novaAresta->nomeTecDestino, r->nomeTecDestino.nome);
                novaAresta->peso = r->peso;

                // Adicionando nova aresta na lista linear
                novaAresta->proxAresta = vAux->listaLinear;
                vAux->listaLinear = novaAresta;

                vAux->grau++;
                vAux->grauSaida++;
                grafo->numArestas++;
            }

            // Se já existe uma aresta saindo da tec Origem e indo para a de Destino, ele retorna
            else if(strcmp(a->nomeTecDestino, r->nomeTecDestino.nome) == 0) {
                return; 
            }

            else {
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


/*
    Função que adiociona um registro no grafo transposto
*/ 
void adicionaRegistroTransposto(Registro *r, Grafo *grafo) { 
    
    // Verifica se o grafo está vazio, se estiver, ele cria os primeiros vértices
    if(grafo->primeiroElem == NULL) {
        // Cria o vértice da tecnologia de destino
        Vertice *v1 = malloc(sizeof(Vertice));
        v1->nomeTec = malloc(r->nomeTecDestino.tam * sizeof(char));
        strcpy(v1->nomeTec, r->nomeTecDestino.nome);
        v1->grupo = r->grupo;   // Inicializa o grupo da tecnologia de destino igual ao da de origem

        // Como o grafo é transposto a aresta sai do vértice da tecnologia Destino
        v1->grauEntrada = 0;
        v1->grauSaida = 1; 
        v1->grau = 1;

        // Cria a lista de arestas do vértice da tecnologia de origem
        v1->listaLinear = malloc(sizeof(Aresta));
        v1->listaLinear->nomeTecDestino = malloc(r->nomeTecOrigem.tam * sizeof(char)); 
        strcpy(v1->listaLinear->nomeTecDestino, r->nomeTecOrigem.nome);     // primeira aresta é para a tecnologia de origem do registro
        v1->listaLinear->peso = r->peso;
        v1->listaLinear->proxAresta = NULL; // inicialmente só possui uma aresta

        // Cria o vértice da tecnologia de origem
        Vertice *v2 = malloc(sizeof(Vertice));  
        v2->nomeTec = malloc(r->nomeTecOrigem.tam * sizeof(char));
        strcpy(v2->nomeTec, r->nomeTecOrigem.nome);

        // Grafo transposto, logo a aresta chega no vértice da tecnologia de origem
        v2->grauEntrada = 1; 
        v2->grauSaida = 0;
        v2->grau = 1;

        v2->grupo = r->grupo;
        v2->listaLinear = NULL;     // tec de origem não terá aresta saindo de seu vértice inicialmente
        
        // // Se os nomes das tecnologias forem iguais, só será criado 1 vértice
        // if(strcmp(r->nomeTecOrigem.nome, r->nomeTecDestino.nome) == 0) {
        //     free(v2);   // libera o vértice da tecnologia de destino (já que é a mesma da de origem)

        //     // Adiciona a tecnologia de origem como primeiro vértice do grafo
        //     v1->grauEntrada++;
        //     v1->grau++;
        //     grafo->primeiroElem = v1;
        //     grafo->numVertices++;
        //     grafo->numArestas++;
        // }

        // Se o nome da tecnologia destino for menor 
        if(strcmp(r->nomeTecDestino.nome, r->nomeTecOrigem.nome) < 0) {           
            v1->proxElem = v2;          // v1 aponta para v2
            v2->proxElem = NULL;
            grafo->primeiroElem = v1;   // v1 é o primeiro elemento do grafo
            grafo->numVertices += 2;
            grafo->numArestas++;
        }

        // Se o nome da tecnologia de origem for menor
        else if(strcmp(r->nomeTecOrigem.nome, r->nomeTecDestino.nome) < 0) { 
            v2->proxElem = v1;          // v2 aponta para v1
            v1->proxElem = NULL;
            grafo->primeiroElem = v2;   // v2 é o primeiro elemento do grafo
            grafo->numVertices += 2;
            grafo->numArestas++;
        }

        return;
    } 

    // Se o grafo não estiver vazio, busca se já existem vértices com os nomes da tecnologia de destino
    Vertice *vAux = buscaVertice(grafo->primeiroElem, r->nomeTecDestino.nome);

    // Se o vértice ainda não existe e for ser o primeiro do grafo
    if(vAux == NULL) {
        Vertice *v1 = malloc(sizeof(Vertice));
        v1->nomeTec = malloc(r->nomeTecDestino.tam * sizeof(char));
        strcpy(v1->nomeTec, r->nomeTecDestino.nome);

        // no grafo transposto a aresta sai da tec de destino
        v1->grau = 1;
        v1->grauEntrada = 0; 
        v1->grauSaida = 1;

        v1->grupo = r->grupo;   // grupo da tec de destino começa igual ao grupo da tec de origem do registro

        // Adiciona a aresta que sai da tecnologia de Destino e vai para a de Origem
        v1->listaLinear = malloc(sizeof(Aresta));
        v1->listaLinear->nomeTecDestino = malloc(r->nomeTecOrigem.tam * sizeof(char));
        strcpy(v1->listaLinear->nomeTecDestino, r->nomeTecOrigem.nome);
        v1->listaLinear->peso = r->peso;
        v1->listaLinear->proxAresta = NULL; // só possui uma aresta até o momento

        // O próximo elemento depois de v1 vai ser o vértice que até agora era o 1°
        v1->proxElem = grafo->primeiroElem;

        // O primeiro vértice do grafo passa a ser v1
        grafo->primeiroElem = v1;

        grafo->numArestas++;
        grafo->numVertices++;
    }

    // Se o vértice da tecnologia de destino já existir
    else if(strcmp(r->nomeTecDestino.nome, vAux->nomeTec) == 0) {
        // Se o vértice ainda não tiver nenhuma aresta saindo dele
        if(vAux->listaLinear == NULL) {
            // Cria a primeira aresta que vai para o vértice da tec de origem
            vAux->listaLinear = malloc(sizeof(Aresta));
            vAux->listaLinear->nomeTecDestino = malloc(r->nomeTecOrigem.tam * sizeof(char));
            strcpy( vAux->listaLinear->nomeTecDestino, r->nomeTecOrigem.nome);
            vAux->listaLinear->peso = r->peso;
            vAux->listaLinear->proxAresta = NULL;

            vAux->grau++;
            vAux->grauSaida++;
            grafo->numArestas++;
        }

        // Se já existir vértice da tec destino, verifica se já existe uma aresta para a tec de origem (já que é transposto)
        else {
            Aresta *a = buscaAresta(vAux->listaLinear, r->nomeTecOrigem.nome);

            // Se a aresta ainda não existe e for ser a primeira da lista linear de arestas:
            if(a == NULL) {
                // Criando aresta que chega na tec de origem do registro
                Aresta *novaAresta = malloc(sizeof(Aresta));
                novaAresta->nomeTecDestino = malloc(r->nomeTecOrigem.tam * sizeof(char));
                strcpy(novaAresta->nomeTecDestino, r->nomeTecOrigem.nome);
                novaAresta->peso = r->peso;

                // Adicionando nova aresta no começo de lista de arestas do vértice
                novaAresta->proxAresta = vAux->listaLinear;
                vAux->listaLinear = novaAresta;

                vAux->grau++;
                vAux->grauSaida++;
                grafo->numArestas++;
            }

            // Se já existe uma aresta saindo da tec Destino do registro e indo para a de Origem, ele retorna
            else if(strcmp(a->nomeTecDestino, r->nomeTecOrigem.nome) == 0) {
                return; 
            }

            // Se não tem a aresta e ela não for ser a primeira da lista de arestas, ele cria e adiciona uma nova aresta
            else {
                // Criando aresta que chega na tec de origem do registro
                Aresta *novaAresta = malloc(sizeof(Aresta));
                novaAresta->nomeTecDestino = malloc(r->nomeTecOrigem.tam * sizeof(char));
                strcpy(novaAresta->nomeTecDestino, r->nomeTecOrigem.nome);
                novaAresta->peso = r->peso;

                // Adicionando nova aresta na lista de arestas da tec de destino
                novaAresta->proxAresta = a->proxAresta;
                a->proxAresta = novaAresta;

                vAux->grau++;
                vAux->grauSaida++;
                grafo->numArestas++;   
            }
            
        }

    }
    
    // Se ainda não tem um vértice da tecnologia de destino do registro
    else { 
        // Cria vértice da tec de destino 
        Vertice *v1 = malloc(sizeof(Vertice));
        v1->nomeTec = malloc(r->nomeTecDestino.tam * sizeof(char));
        strcpy(v1->nomeTec, r->nomeTecDestino.nome);
        v1->grau = 1;
        v1->grauEntrada = 0;
        v1->grauSaida = 1;
        v1->grupo = r->grupo; // grupo começa sendo igual ao da tec de origem

        // Adiciona a aresta que chega na tecnologia de origem
        v1->listaLinear = malloc(sizeof(Aresta));
        v1->listaLinear->nomeTecDestino = malloc(r->nomeTecOrigem.tam * sizeof(char));
        strcpy(v1->listaLinear->nomeTecDestino, r->nomeTecOrigem.nome);
        v1->listaLinear->peso = r->peso;
        v1->listaLinear->proxAresta = NULL;

        // O próximo elemento depois de v1
        v1->proxElem = vAux->proxElem;

        // O elemento antes de v1 aponta para ele
        vAux->proxElem = v1;

        grafo->numArestas++;
        grafo->numVertices++;
    }

    // Agora busca pela tecnologia de origem
    vAux = buscaVertice(grafo->primeiroElem, r->nomeTecOrigem.nome);
    
    // Se o vértice não existe e se o nome da tecnologia de origem for ser o primeiro do grafo
    if(vAux == NULL) {
        // Cria vértice da tecnologia de origem
        Vertice *v2 = malloc(sizeof(Vertice));
        v2->nomeTec = malloc(r->nomeTecOrigem.tam * sizeof(char));
        strcpy(v2->nomeTec, r->nomeTecOrigem.nome);

        // Como o grafo é transposto a aresta chega no vértice da tecnologia de origem
        v2->grau = 1;
        v2->grauEntrada = 1;
        v2->grauSaida = 0;
        v2->listaLinear = NULL;

        v2->grupo = r->grupo;

        // O próximo elemento depois de v2 vai ser o vértice que até agora era o 1°
        v2->proxElem = grafo->primeiroElem;

        // O primeiro vértice do grafo passa a ser v2
        grafo->primeiroElem = v2;

        grafo->numVertices++;
    }

    // Se o vértice já existir
    else if(strcmp(r->nomeTecOrigem.nome, vAux->nomeTec) == 0) { 
        /* Atualiza o grupo já que caso esse vértice da tecnologia de origem tiver
        sido criado quando a tecnologia era de destino de um registro o grupo não for o certo*/
        vAux->grupo = r->grupo;     

        // Atualiza as arestas que chegam no vértice
        vAux->grau++;
        vAux->grauEntrada++;
    }
    
    // Se ainda não tem um vértice da tecnologia de origem do registro no grafo
    else {  
        // Cria o vértice da tecnologia de origem
        Vertice *v2 = malloc(sizeof(Vertice));
        v2->nomeTec = malloc(r->nomeTecOrigem.tam * sizeof(char));
        strcpy(v2->nomeTec, r->nomeTecOrigem.nome);

        // Inicializa seus dados
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

// Função recursiva que desaloca as arestas de um vértice
void liberaArestas(Aresta *a) {
    if(a->proxAresta != NULL) {
        liberaArestas(a->proxAresta);   // liberando as próximas arestas (recursão)
    } 

    // liberando a aresta atual  
    free(a);
}

// Função recursiva que desaloca os vértices de um grafo
void liberaVertices(Vertice *v) {
    // Libera as arestas do vértice
    if(v->listaLinear != NULL) {
        liberaArestas(v->listaLinear);
    }

    // Libera os próximos vértices
    if(v->proxElem != NULL) {
        liberaVertices(v->proxElem);
    }

    // Libera o vértice atual
    free(v);
}

// Função que desaloca o grafo
void liberaGrafo(Grafo *grafo) {

    if(grafo->primeiroElem != NULL) {
        liberaVertices(grafo->primeiroElem);
    }

    // Desaloca o grafo
    free(grafo);
}

void imprimeTecAresta(Aresta *a) {
    printf("%s", a->nomeTecDestino);

    if(a->proxAresta != NULL) {
        printf(", ");
        imprimeTecAresta(a->proxAresta); 
    }      
}

// Funções para a pilha -----------------------------------------
Stack* createStack(int size) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = -1;
    stack->array = (int*)malloc(size * sizeof(int));
    return stack;
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, int item) {
    stack->array[++stack->top] = item;
}


int pop(Stack* stack) {
    if (isEmpty(stack))
        return -1;
    return stack->array[stack->top--];
}
//--------------------------------------------------------------

// void preenche_ordem(Grafo* grafo, Stack* stack, int passou[], int i){
//     passou[i] = 1;

//     Aresta* aresta = grafo->primeiroElem->listaLinear;
//     while (aresta != NULL) {
//         int destino = encontrarOuCriarVertice(grafo, aresta->nomeTecDestino);
//         if (!passou[destino]) {
//             preenche_ordem(grafo, stack, passou, destino);
//         }

//         aresta = aresta->proxAresta;
//     }

//     push(stack, i);
// }
//--------------------------------------------------------

Vertice *buscaVerticeIndex(Vertice *v, char *nomeTec, int *index) {
    //*index += 1;

    // Se o nome da tecnologia do vértice for maior que a tecnologia buscada
    if(strcmp(v->nomeTec, nomeTec) > 0) {
        return NULL;    // retorna que não achou o vértice da tecnologia
    } 
    
    // Se o nome das tecnologias for igual, ele retorna o vértice 
    if(strcmp(v->nomeTec, nomeTec) == 0) {
        return v;
    }

    /* Se chegou no fim dos vértices retorna o vértice atual ("último vértice") 
       O vértice da tecnologia buscada será criado e adicionado depois do último */
    if (v->proxElem == NULL) {
        return v;
    }

    // Chamada recursiva (busca a tecnologia nos próximos vértices)
    Vertice *v2 = buscaVerticeIndex(v->proxElem, nomeTec, index);

    // Se o retorno da chamada recursiva for nulo, retorna o vértice atual
    if(v2 == NULL) {
        return v;
    }       

    // Senão, retorna o que veio da recursão
    else {
        return v2;
    }   
}

// Função para encontrar ou criar um vértice com o nome especificado
// int encontrarOuCriarVertice(Grafo *grafo, char *nomeTecnologia) {
//     // Buscar o vértice no grafo
//     Vertice *v = buscaVertice(grafo->primeiroElem, nomeTecnologia);
//     //se existir o vértice, retorna o índice dele
//     if(v != NULL || strcmp(v->nomeTec, nomeTecnologia) == 0 || v->listaLinear != NULL){
//         // int *index = malloc(sizeof(int));
//         // int i = buscaVerticeIndex(grafo->primeiroElem, nomeTecnologia, index);
//         return i;
//     } else{
//         // Vértice não encontrado, criar um novo
//         grafo->numVertices++;
//         grafo->primeiroElem = (Vertice *)realloc(grafo->primeiroElem, grafo->numVertices * sizeof(Vertice));
//         int novoIndice = grafo->numVertices - 1;
//         grafo->primeiroElem[novoIndice].nomeTec= strdup(nomeTecnologia);
        
//         // Inicializar os campos do novo vértice
//         grafo->primeiroElem->grupo = -4;
//         grafo->primeiroElem->grauEntrada = 0;
//         grafo->primeiroElem->grauSaida = 0;
//         grafo->primeiroElem->grau = 0;
//         grafo->primeiroElem->listaLinear = NULL;
//         grafo->primeiroElem->proxElem = 0;

//         return novoIndice;
//     }
// }




// Adicionar uma aresta direcionada entre dois vértices no grafo.
// void addEdge(Grafo* graph, char *src, char *dest) {
//     int srcIndex = buscaVertice(graph->primeiroElem, src) - graph->primeiroElem;
//     int destIndex = buscaVertice(graph->primeiroElem, dest) - graph->primeiroElem;

//     if (srcIndex >= 0 && destIndex >= 0) {
//         Aresta* newEdge = (Aresta*)malloc(sizeof(Aresta));
//         newEdge->nomeTecDestino = strdup(dest);
//         newEdge->peso = 1; // Ajuste conforme necessário
//         newEdge->proxAresta = graph->primeiroElem[srcIndex].listaLinear;
//         graph->primeiroElem[srcIndex].listaLinear = newEdge;
//     } else {
//         fprintf(stderr, "Erro: Vértice de origem ou destino não encontrado ao adicionar aresta.\n");
//     }
// }

// Gerar o grafo transposto, que é um grafo resultante da inversão das direções de todas as arestas do grafo original.
// Grafo* getTranspose(Grafo* graph) {
//     Grafo* transpose = (Grafo*)malloc(sizeof(Grafo));
//     transpose->numVertices = graph->numVertices;
//     transpose->numArestas = graph->numArestas;
//     transpose->primeiroElem = (Vertice*)malloc(graph->numVertices * sizeof(Vertice));

//     for (int i = 0; i < graph->numVertices; ++i) {
//         transpose->primeiroElem[i].nomeTec = graph->primeiroElem[i].nomeTec;
//         transpose->primeiroElem[i].listaLinear = NULL;
//         transpose->primeiroElem[i].proxElem = NULL;
//     }

//     for (int v = 0; v < graph->numVertices; ++v) {
//         Aresta* current = graph->primeiroElem[v].listaLinear;
//         while (current != NULL) {
//             Vertice* destinationVertex = buscaVertice(transpose->primeiroElem, current->nomeTecDestino);

//             if (destinationVertex != NULL) {
//                 Aresta* newEdge = (Aresta*)malloc(sizeof(Aresta));
//                 newEdge->nomeTecDestino = strdup(graph->primeiroElem[v].nomeTec);
//                 newEdge->peso = 1; // Ajuste conforme necessário
//                 newEdge->proxAresta = destinationVertex->listaLinear;
//                 destinationVertex->listaLinear = newEdge;
//             } else {
//                 fprintf(stderr, "Erro: Vértice de destino não encontrado ao adicionar aresta invertida.\n");
//             }

//             current = current->proxAresta;
//         }
//     }

//     return transpose;
// }

// // Função auxiliar para a busca em profundidade (DFS).
// void DFSUtil(Vertice *vertices, int vertex, int visited[], Stack* stack, Grafo* grafo) {
//     visited[vertex] = 1;
//     Aresta *current = vertices[vertex].listaLinear;

//     while (current != NULL) {
//         int destino = encontrarOuCriarVertice(grafo, current->nomeTecDestino);
//         if (!visited[destino]) {
//             DFSUtil(vertices, destino, visited, stack, grafo);
//         }
//         current = current->proxAresta;
//     }
//     push(stack, vertex);
// }

// Inicia a busca em profundidade a partir de um vértice específico.
// void DFS(Vertice *vertices, int vertex, int visited[]) {
//     visited[vertex] = 1;
//     printf("%s ", vertices[vertex].nomeTec);

//     Aresta *current = vertices[vertex].listaLinear;
//     while (current != NULL) {
//         int destinationIndex = buscaVertice(vertices, current->nomeTecDestino) - vertices;
//         if (!visited[destinationIndex])
//             DFS(vertices, destinationIndex, visited);
//         current = current->proxAresta;
//     }
// }

// Função que verifica se o grafo é conexo
// int verificaConexo(Grafo *graph){
//     Stack* stack = (Stack*)malloc(sizeof(Stack));
//     stack->array = (int*)malloc(graph->numVertices * sizeof(int));
//     stack->top = -1;

//     int* visited = (int*)malloc(graph->numVertices * sizeof(int));

//     for (int i = 0; i < graph->numVertices; ++i)
//         visited[i] = 0;

//     // Realiza a primeira passagem do algoritmo de Kosaraju
//     for (int i = 0; i < graph->numVertices; ++i) {
//         if (!visited[i])
//             DFSUtil(graph->primeiroElem, i, visited, stack, graph);
//     }

//     Grafo* transpose = getTranspose(graph);

//     for (int i = 0; i < graph->numVertices; ++i)
//         visited[i] = 0;

//     while (!isEmpty(stack)) {
//         int vertex = pop(stack);
//         if (!visited[vertex]) {
//             DFS(transpose->primeiroElem, vertex, visited);
//             printf("\n");
//         }
//     }

//     free(visited);
//     free(stack);


//     free(stack->array);
//     free(stack);

//     return 0;
// }


void achaMenor(estruturaAux *aux, int numVertices, int *min, int *indiceMin) {
    // Achando o vértice com a menor distância
    for(int i = 0; i < numVertices; i ++) {
        if(aux[i].visitado == 0) {
            if(aux[i].distancia < *min) {
                *indiceMin = i;
                *min = aux[i].distancia;
            }
        }
    }
}
    

//Função para encontrar o índice do vértice com a menor distância ainda não incluído no caminho mínimo
int menorDistancia(int index, estruturaAux *aux, int numVertices) {
    int min = INFINITO;
    int indiceMin = -1;
    
    if(aux[index].v.listaLinear == NULL) {
        return -1;
    }

    Aresta a = *aux[index].v.listaLinear;
    for(int i = 0; i < aux[index].v.grauSaida; i++) {
        for(int j = 0; j < numVertices; j ++) {
            // Se o vértice de index j for adjacente ao vertice w
            if(strcmp(a.nomeTecDestino, aux[j].v.nomeTec) == 0) {
                if(aux[j].distancia > a.peso + aux[index].distancia)
                    aux[j].distancia = a.peso + aux[index].distancia;
            }
        }

        if(i != aux[index].v.grauSaida - 1)
            a = *a.proxAresta;
    }

    // Achando o vértice com a menor distância
    achaMenor(aux, numVertices, &min, &indiceMin);

    aux[indiceMin].visitado = 1;
    return indiceMin;
}

//Retorna a menor distância entre dois vértices. Se eles não forem conectados, retorna -1
int algoritmoDijkstra(Grafo *grafo, char *tecOrigem, char *tecDestino) {
    estruturaAux *aux = malloc(grafo->numVertices * sizeof(estruturaAux));

    int indexOrigem = -1;
    int indexDestino = -1;

    // Primeiro vértice do grafo
    Vertice v = *grafo->primeiroElem;

    for (int i = 0; i < grafo->numVertices; i++) {
        aux[i].distancia = INFINITO; // distância "infinita"
        aux[i].visitado = 0;        // nenhum vértice foi "adicionado"/"visitado" ainda
        
        aux[i].v = v;   // "copia" os vértices do grafo na estrutura auxiliar

        // verifica se ainda não chegou no fim dos vértices. Se não, passa para o próx vértice
        if(i != grafo->numVertices - 1)
            v = *v.proxElem;

        if(strcmp(aux[i].v.nomeTec, tecOrigem) == 0) {
            indexOrigem = i;    // acha o índice do vértice da tecnologia de origem
        }
        if(strcmp(aux[i].v.nomeTec, tecDestino) == 0) {
            indexDestino = i;   // acha o índice do vértice da tecnologia de destino
        }
    }

    // Se a tecnologia de origem ou destino não existe no grafo
    if(indexOrigem == -1 || indexDestino == -1) {
        free(aux);
        return -1;
    }

    //printf("\naqui");

    // Primeiro vértice a ser adicionado na lista de visitados é o da tecnologia de origem
    v = aux[indexOrigem].v;
    aux[indexOrigem].distancia = 0;     // distância da origem até a origem é 0
    aux[indexOrigem].visitado = 1;

    int index = indexOrigem;
    for(int i = 0; i < grafo->numVertices - 1; i++) {
        index = menorDistancia(index, aux, grafo->numVertices);
        if(index == -1) {
            int min, indiceMin;
            achaMenor(aux, grafo->numVertices, &min, &indiceMin);
            aux[indiceMin].visitado = 1;
            index = indiceMin;
        }
    }

    int distancia = aux[indexDestino].distancia;

    if(distancia != INFINITO) {
        return distancia;
    } else {
        return -1;
    }
}
