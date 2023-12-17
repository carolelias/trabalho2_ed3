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

    // Alocando espaço para um registro auxiliar e um char auxiliar
    Registro *rAux = malloc(sizeof(Registro));
    char *cAux = malloc(sizeof(char));

    // Criando o grafo transposto
    Grafo *grafo = criaGrafo();

    //Lendo os registros do arquivo de dados
    for(int i = 0; i < entrada->rC.proxRRN; i++) {
        // Lendo o registro do arquivo de dados e armazenando no registro auxiliar
        if(leRegistroBinario(entrada->p, rAux, cAux, i) == 0) {
            // Se nenhuma das tecnologias for nula, adiciona o registro no grafo
            if((rAux->nomeTecOrigem.tam != 0 && rAux->nomeTecDestino.tam != 0) && (rAux->nomeTecOrigem.nome[0] != '$' && rAux->nomeTecDestino.nome[0] != '$')) {
                //adiciona os registros do arquivo de dados no grafo transposto transpondo as arestas
                adicionaRegistroTransposto(rAux, grafo);
            }
        }
    }

    // variável onde será armazenado o nome da tecnologia que o usuário está pesquisando
    char *valorCampo = malloc(sizeof(char));

    // lê as tecnologias n vezes
    for(int i = 0; i < n; i++) {
        // lendo a tecnologia que o usuário deseja pesquisar
        scanf("%s", valorCampo);
        // removendo as aspas da string usando a função memmove que move o conteúdo da string para a esquerda
        memmove(valorCampo, valorCampo + 1, strlen(valorCampo) - 2);
        // adicionando o caractere nulo no final da string para indicar o fim da string
        valorCampo[strlen(valorCampo) - 2] = '\0';

        // buscando o vértice que contém a tecnologia que o usuário deseja pesquisar
        Vertice *v = buscaVertice(grafo->primeiroElem, valorCampo);  

        // se o vértice correspondente a tecnologia pesquisada não existir, imprime "Registro Inexistente" 
        if(v == NULL) {
            printf("Registro Inexistente\n");
        }
        
        //se o vértice existir mas não tiver a tecnologia pesquisada, imprime "Registro Inexistente"
        else if(strcmp(v->nomeTec, valorCampo) != 0) {
            printf("Registro Inexistente\n");
        }

        // se o vértice existir, mas não tiver nenhuma aresta, imprime "Registro Inexistente"   
        else if(v->listaLinear == NULL) {
            printf("Registro Inexistente\n");
        }
        // se o vértice existir e tiver arestas, imprime as arestas
        else {
            printf("%s: ", valorCampo);
            // imprime as arestas do vértice que contém a tecnologia pesquisada
            imprimeTecAresta(v->listaLinear);
            printf("\n\n");
        }
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


//funcionalidade 12
void caminhoMaisCurto(char *arq1) {
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

    // Lendo as tecnologias de origem e destino
    char tecOrigem[20];
    char tecDestino[20];

    int n;
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
       scanf("%s %s", tecOrigem, tecDestino);

        int distancia = algoritmoDijkstra(grafo, tecOrigem, tecDestino);
        
        printf("%s %s: ", tecOrigem, tecDestino);
        if(distancia == -1){
            printf("CAMINHO INEXISTENTE.\n");
        } else {
            printf("%d", distancia);
        } 
    }
    
    

    liberaGrafo(grafo); // desaloca todas as estruturas do grafo (ele mesmo, seus vértices, arestas, ...)
    free(cAux);
    free(rAux);
    free(entrada);
    fclose(p);  // fecha o arquivo binário
}