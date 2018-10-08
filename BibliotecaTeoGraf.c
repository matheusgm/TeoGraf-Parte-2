#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FilaLista.h"
#include "PilhaLista.h"
#include "Lista.h"
#include "FuncoesAuxiliares.h"
#include "typeVertice.h"
#include <stdbool.h>
#define NUMERO_LINHA 1000

#include <stdbool.h>
#include <math.h>
#include "Prim.h"
#include "Dijkstra.h"

typedef struct arvore Arvore;
struct arvore{
    int nivel;
    int pai;
};

typedef struct marcacao2 Marcacao2;
struct marcacao2{
    int numVertice;
    int numMarcacao;
};

typedef struct endVetorPrincipal EndVetorPrincipal;
struct endVetorPrincipal{
    int qntMarcacao;
    Marcacao2* endereco;
};

// ----------------------------------------------------- INICIO DE GERAR GRAFOS -----------------------------------------------------
Vertice* geraListaAdjacencia(FILE *arq,char *Linha,char *result, int numVertices, int *numArestas, int *vetorGraus, bool *isUnweighted,bool *gotNegativeEdges){

    int i,pri,seg,ultimoCaracter;
    float peso;
    Vertice *vetorVertice;
    Elemento *lst;
    char* cha;
    vetorVertice = (Vertice*) malloc(numVertices * sizeof(Vertice));
    for(i = 0; i < numVertices; i++){
        vetorVertice[i].n = i+1;
        vetorVertice[i].adjancencia = lst_cria();
        vetorGraus[i] = 0;
    }

    if(numVertices > 10000000){
        cha = "	";
    }else{
        cha = " ";
    }

    printf("Vertices: %d\n",numVertices);
    i = 0;
    int r=0;
    *isUnweighted = true;
    *gotNegativeEdges = false;
    while (!feof(arq))
    {

        // Lê uma linha (inclusive com o '\n')
        result = fgets(Linha, NUMERO_LINHA-1, arq);  // o 'fgets' lê até NUMERO_LINHA caracteres ou até o '\n'
        ultimoCaracter = strlen(result)-1;
        // Verificar e tirar o \n caso exista
        if(result[ultimoCaracter] == '\n'){
            result[ultimoCaracter] = '\0';
        }

        pri = atoi(strtok(result,cha));
        seg = atoi(strtok('\0',cha));

        peso = strtof(strtok('\0',cha),NULL);
//        printf("Hello!\n");

        if(peso != 0)
         *isUnweighted = false;
        if(peso < 0)
         *gotNegativeEdges = true;
//        printf("%d %d %.2f\n",pri,seg,peso);
        if(pri > numVertices || seg > numVertices){
//            printf("Erro! %s com numero incorreto\n", result);
            r++;
            continue;
        }

        // Insere o segundo vertice na lista adjacencia do primeiro vertice
        lst = vetorVertice[pri-1].adjancencia;
        lst = lst_insere(lst,seg,peso);
        //lst_imprime(lst);
        vetorVertice[pri-1].adjancencia = lst;

        // Insere o primeiro vertice na lista adjacencia do segundo vertice
        lst = vetorVertice[seg-1].adjancencia;
        lst = lst_insere(lst,pri,peso);
        vetorVertice[seg-1].adjancencia = lst;

        vetorGraus[pri-1]++;
        vetorGraus[seg-1]++;

        i++;
        (*numArestas)++;
//        if(i == 11)
//         break;
    }
//    printf("Chegou Aqui\n");
    printf("Linhas erradas: %d\n",r);
    return vetorVertice;
}

// ----------------------------------------------------- FIM DE GERAR GRAFOS -----------------------------------------------------

// ----------------------------------------------------- INICIO DE IMPRIMIR GRAFOS -----------------------------------------------------
// Imprime o Grafo na forma de Lista de Adjacencia
void imprimeListaAdjacencia(Vertice *vetorVertice, int numVertices){
    int i;
    for(i = 0; i < numVertices; i++){
        printf("Vertice %d: ",i+1);
        lst_imprime(vetorVertice[i].adjancencia);
        printf("\n");
    }
}
// ----------------------------------------------------- FIM DE IMPRIMIR GRAFOS -----------------------------------------------------

// ----------------------------------------------------- INICIO ARQUIVOS DE SAIDA -----------------------------------------------------
// Funcao para gerar o arquivo de Arvore
void gerarArquivoArvore(Arvore *arvore, int tam){
    FILE *arqSaida;
    int i;
    if((arqSaida = fopen("Arvore.txt", "wb")) == NULL)
    {
        printf("Erro na abertura do arquivo");
        exit(1);
    }
    for(i=0;i<tam;i++){
        if(arvore[i].pai == 0){
            fprintf(arqSaida,"Vertice: %d -> Nivel: %d -> Pai: Raiz\r\n",i+1,arvore[i].nivel);
        }else if(arvore[i].pai != -1){
            fprintf(arqSaida,"Vertice: %d -> Nivel: %d -> Pai: %d\r\n",i+1,arvore[i].nivel,arvore[i].pai);
        }
    }

    fclose(arqSaida);

}

// Gera um Arquivo de Saida com o Numero Total de Vertices, Numero Total de Arestas, Grau Maximo, Grau Minimo, Grau Medio e Mediana dos Graus
void gerarArquivoSaida(int numeroVertice, int numArestas, int gMax, int gMin, float gMediana, float gMedio, char* nomeArqSaida){
    FILE *arqSaida;
    if((arqSaida = fopen(nomeArqSaida, "wb")) == NULL)
    {
        printf("Erro na abertura do arquivo");
        exit(1);
    }

    fprintf(arqSaida,"Numero de Vertices: %d\r\n",numeroVertice);
    fprintf(arqSaida,"Numero de Arestas: %d\r\n", numArestas);
    fprintf(arqSaida,"Grau Minimo: %d\r\n",gMin);
    fprintf(arqSaida,"Grau Maximo: %d\r\n",gMax);
    fprintf(arqSaida,"Grau Media: %.2f\r\n",gMedio);
    fprintf(arqSaida,"Mediana do Grau: %.2f\r\n",gMediana);

    fclose(arqSaida);

}
// ----------------------------------------------------- FIM ARQUIVOS DE SAIDA -----------------------------------------------------

// ----------------------------------------------------- INICIO DE BUSCA -----------------------------------------------------
// BFS de Lista de Adjacencia
int* BFSListaAdjacencia(Vertice *Grafo, int s, int tam){
    int v,i;
    Elemento *p;
    Arvore *vetorArvore = malloc(tam*sizeof(Arvore));
    int *vetorMarcacao = malloc(tam*sizeof(int));
    for(i = 0; i < tam; i++){
        vetorMarcacao[i] = 0;
        vetorArvore[i].nivel = -1;
        vetorArvore[i].pai = -1;
    }
    vetorMarcacao[s-1]=1;
    vetorArvore[s-1].nivel = 0;
    vetorArvore[s-1].pai = 0;
    Fila *Q = fila_cria();
    fila_insere(Q,s);
    while(!fila_vazia(Q)){
        v = fila_retira(Q);
        for(p = Grafo[v-1].adjancencia; p!= NULL; p=proximaLista(p)){
            int w = getItem(p);
            if(vetorMarcacao[w-1]==0){
//                printf("%d\n",w);
                vetorMarcacao[w-1]=1;
                fila_insere(Q,w);
                vetorArvore[w-1].nivel = vetorArvore[v-1].nivel + 1;
                vetorArvore[w-1].pai = v;
            }
        }
    }

    gerarArquivoArvore(vetorArvore,tam);
    free(vetorArvore);
//    free(vetorMarcacao);
    return vetorMarcacao;
}

// DFS de Lista de Adjacencia
int* DFSListaAdjacencia(Vertice *Grafo, int s, int tam){
    int v,i;
    Pilha *P = pilha_cria();
    Arvore *vetorArvore = malloc(tam*sizeof(Arvore));
    int *vetorMarcacao = malloc(tam*sizeof(int));
    Elemento *p;
    for(i = 0; i < tam; i++){
        vetorMarcacao[i] = 0;
    }
    vetorArvore[s-1].nivel = 0;
    vetorArvore[s-1].pai = 0;
    pilha_push(P,s);
    while(!pilha_vazia(P)){
        v = pilha_pop(P);
        if(vetorMarcacao[v-1] != 1){
            vetorMarcacao[v-1] = 1;
            for(p = Grafo[v-1].adjancencia; p!= NULL; p=proximaLista(p)){
                int w = getItem(p);
                pilha_push(P,w);
                if(vetorMarcacao[w-1] != 1){
                    vetorArvore[w-1].nivel = vetorArvore[v-1].nivel + 1;
                    vetorArvore[w-1].pai = v;
                }
            }
        }
    }
//    gerarArquivoArvore(vetorArvore,tam);
    free(vetorArvore);
    return vetorMarcacao;
}

// BFS de Lista de Adjacencia para utilizar na busca de Componentes Conexos
void BFSListaAdjacencia02(Vertice *Grafo, int s, int tam, Marcacao2* vetorMarcacao, int numMarcado){
    int v;
    Elemento *p;
    Arvore *vetorArvore = malloc(tam*sizeof(Arvore));

    vetorMarcacao[s-1].numMarcacao=numMarcado;
    vetorArvore[s-1].nivel = 0;
    vetorArvore[s-1].pai = 0;
    Fila *Q = fila_cria();
    fila_insere(Q,s);
    while(!fila_vazia(Q)){
        v = fila_retira(Q);
        for(p = Grafo[v-1].adjancencia; p!= NULL; p=proximaLista(p)){
            int w = getItem(p);
            if(vetorMarcacao[w-1].numMarcacao==0){
//                printf("%d\n",w);
                vetorMarcacao[w-1].numMarcacao=numMarcado;
                fila_insere(Q,w);
                vetorArvore[w-1].nivel = vetorArvore[v-1].nivel + 1;
                vetorArvore[w-1].pai = v;
            }
        }
    }
//    gerarArquivoArvore(vetorArvore,tam);
    free(vetorArvore);
    return;
}

int BFSListaAdjacencia03(Vertice *Grafo, int s, int tam){
    int v,i;
    int distancia = 0;
    Elemento *p;
    Arvore *vetorArvore = malloc(tam*sizeof(Arvore));
    int *vetorMarcacao = malloc(tam*sizeof(int));
    for(i = 0; i < tam; i++){
        vetorMarcacao[i] = 0;
        vetorArvore[i].nivel = -1;
        vetorArvore[i].pai = -1;
    }
    vetorMarcacao[s-1]=1;
    vetorArvore[s-1].nivel = 0;
    vetorArvore[s-1].pai = 0;
    Fila *Q = fila_cria();
    fila_insere(Q,s);
    while(!fila_vazia(Q)){
        v = fila_retira(Q);
        for(p = Grafo[v-1].adjancencia; p!= NULL; p=proximaLista(p)){
            int w = getItem(p);
            if(vetorMarcacao[w-1]==0){
//                printf("%d\n",w);
                vetorMarcacao[w-1]=1;
                fila_insere(Q,w);
                vetorArvore[w-1].nivel = vetorArvore[v-1].nivel + 1;
                vetorArvore[w-1].pai = v;
                if(vetorArvore[w-1].nivel > distancia){
                    distancia = vetorArvore[w-1].nivel;
                }
            }
        }
    }

//    gerarArquivoArvore(vetorArvore,tam);
    free(vetorArvore);
    free(vetorMarcacao);
    return distancia;
}
// ----------------------------------------------------- FIM DE BUSCA -----------------------------------------------------

// ----------------------------------------------------- INICIO CICLO 1000X -----------------------------------------------------
// Executa, para Lista Adjacencia, a BFS(0) ou DFS(1) 1000x exibindo o tempo media e gerando um arquivo com o tempo em cada ciclo
void milCiclosListaAdjacencia(Vertice *vetorVertice, int numVertices, char* nomeArquivo, int tipo){
    long start,end;
    int i;
    int *vetorMarcacao;
//    double somaTempo = 0;

    FILE* s = fopen(nomeArquivo, "wb");
    fprintf(s,"Ciclo Tempo\r\n");
    if(tipo==0){
        for(i=0;i<1000;i++){
            printf("%d\n",i+1);
            start = getMicrotime();
            vetorMarcacao = BFSListaAdjacencia(vetorVertice,1,numVertices);
            end = getMicrotime();
            free(vetorMarcacao);
//            somaTempo += (double) end-start;
            fprintf(s,"%d %ld\r\n",i+1,end-start);
//          printf("Ciclo: %d -> %ld\r\n",i+1,end-start);
        }
    }
    else if(tipo==1){
        for(i=0;i<1000;i++){
            printf("%d\n",i+1);
            start = getMicrotime();
            vetorMarcacao = DFSListaAdjacencia(vetorVertice,1,numVertices);
            end = getMicrotime();
            free(vetorMarcacao);
//            somaTempo += (double) (end-start);
            fprintf(s,"%d %ld\r\n",i+1,end-start);
//          printf("Ciclo: %d -> %ld\r\n",i+1,end-start);
        }
    }else{
        fprintf(s,"Erro ao inserir o tipo de Busca. Tipos Validos: 0 (BFS) ou 1(DFS)\r\n");
        printf("Erro ao inserir o tipo de Busca. Tipos Validos: 0 (BFS) ou 1(DFS)\n");
    }

    fclose(s);
//    printf("Tempo Medio: %.2f\n",somaTempo/1000);
}

// ----------------------------------------------------- FIM CICLO 1000X -----------------------------------------------------

// ----------------------------------------------------- OUTRAS FUNÇÕES -----------------------------------------------------
void componenteConexa(Vertice *Grafo, int tam){
    int i,h,qntTotalConexo;
    int j = 1;
    int numVerticeComponenteConexa = 0;
    printf("Iniciando Componente Conexa!\n");
    // Inicializa o Vetor de Marcação que contem o numero da marcação e o numero do vertice
    Marcacao2* vetorMarcacaoCC = (Marcacao2*) malloc(tam*sizeof(Marcacao2));
    for(i = 0; i < tam; i++){
        vetorMarcacaoCC[i].numMarcacao=0;
        vetorMarcacaoCC[i].numVertice=i+1;
    }
    printf("Executando BFS!\n");
    // Executa a BFS varias vezes, marcando com o numero da componente conexa
    for(i = 0; i < tam; i++){
        if(vetorMarcacaoCC[i].numMarcacao == 0){
//            printf("%d\n",i+1);
            BFSListaAdjacencia02(Grafo,i+1,tam,vetorMarcacaoCC,j);
            j++;
        }
    }
    printf("Liberando Memoria!\n");
    // Liberar Espaco do Grafo, pois só utilizaremos o vetor de marcação
    for(i=0;i<tam;i++){
        lst_libera(Grafo[i].adjancencia);
    }
    free(Grafo);

    printf("1 MergeSort!\n");
    // Ordena o vetor de marcação em ordem crescente do numero da componente conexa
    mergeSortStruct(vetorMarcacaoCC,0,tam-1);
    qntTotalConexo = vetorMarcacaoCC[tam-1].numMarcacao;

    printf("Guardando Endereco!\n");
    // Cria um vetor para guardar a quantidade de elementos de cada componente conexa e o
    // endereco do inicio da componente conexa no vetor de marcaçao
    EndVetorPrincipal *vet = malloc(qntTotalConexo*sizeof(EndVetorPrincipal));
    for(i = 0; i < tam; i++){
        if(i == 0 || vetorMarcacaoCC[i].numMarcacao != vetorMarcacaoCC[i-1].numMarcacao){
            vet[vetorMarcacaoCC[i].numMarcacao - 1].endereco = &(vetorMarcacaoCC[i]);
        }
        numVerticeComponenteConexa++;
        if(i == tam - 1 || vetorMarcacaoCC[i].numMarcacao != vetorMarcacaoCC[i+1].numMarcacao){
            vet[vetorMarcacaoCC[i].numMarcacao - 1].qntMarcacao = numVerticeComponenteConexa;
            numVerticeComponenteConexa=0;
        }
    }

    printf("2 MergeSort!\n");
    // Ordena o vetor em ordem crescente do numero de quantidade de elemento de cada componente
    mergeSortStructEnd(vet,0,qntTotalConexo-1);

//    printf("Imprimindo...\n");
    // Imprime o numero e a quantidade de vertices de cada componente
//    for(i = qntTotalConexo - 1; i>=0; i--){
//        printf("Total: %d \n",vet[i].qntMarcacao);
//        for(h = 0; h < vet[i].qntMarcacao; h++){
//            printf("%d ",vet[i].endereco[h].numVertice);
//        }
//        printf("\n");
//    }
        printf("Qnt Total Conexo: %d\n",qntTotalConexo);
        printf("Maior Conexo: %d\n",vet[qntTotalConexo-1].qntMarcacao);
        printf("Menor Conexo: %d\n",vet[0].qntMarcacao);
//    for(i = 0; i< tam; i++){
//        printf("%d -> ",vetorMarcacaoCC[i].numMarcacao);
//    }
//    printf("\n");

}

void gerarGraus(int* vetorGraus, int numVertices, int numArestas, char* nomeArqSaida){

    int gMax,gMin;
    float gMedio, gMediana;

    mergeSort(vetorGraus,0,numVertices-1);
    gMax = vetorGraus[numVertices-1];
    gMin = vetorGraus[0];

    if(numVertices % 2 == 0){
        gMediana = (float)(vetorGraus[(int)(numVertices/2)] + vetorGraus[(int)(numVertices/2) + 1])/2;
    }else{
        gMediana = (float)vetorGraus[(int)(numVertices/2)];
    }

    gMedio = (numArestas*2)/(float)numVertices;

    gerarArquivoSaida(numVertices, numArestas, gMax, gMin, gMediana, gMedio, nomeArqSaida);

}

int diametroGrafo(Vertice *Grafo, int tam ){
    int maiorDiametro = 0;
    int i,diametro;

    for(i=0;i<tam;i++){
        diametro = BFSListaAdjacencia03(Grafo,i+1,tam);
        if(diametro > maiorDiametro){
            maiorDiametro = diametro;
        }
    }
    return maiorDiametro;

}

char** pesquisadores(int tam){

    int indice,ultimoCaracter;
    char *nome;
    FILE* arq;
    char Linha[NUMERO_LINHA];
    char *result;
    char **nomes;

    // Abre um arquivo TEXTO para LEITURA
    arq = fopen("rede_colaboracao_vertices.txt", "rt");
    if (arq == NULL)  // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }

    nomes = (char**) malloc((tam+1)*sizeof(char*));

    while (!feof(arq))
    {
        // Lê uma linha (inclusive com o '\n')
        result = fgets(Linha, NUMERO_LINHA-1, arq);  // o 'fgets' lê até NUMERO_LINHA caracteres ou até o '\n'
        ultimoCaracter = strlen(result)-1;
        // Verificar e tirar o \n caso exista
        if(result[ultimoCaracter] == '\n'){
            result[ultimoCaracter] = '\0';
        }

        indice = atoi(strtok(result,","));
        nome = strtok('\0',",");

        int l = strlen(nome);
        char *n = (char*) malloc((l+1)*sizeof(char));
        strcpy(n,nome);
        nome[l-1] = '\0';
        nomes[indice] = n;

    }

//    int j;
//    for(j=1;j<=tam;j++){
//        printf("%s\n",nomes[j]);
//    }
    return nomes;

}

void redeColaboracao(){
    FILE *arq;
    char Linha[NUMERO_LINHA];
    char *result;
    int numVertices;
    int numArestas = 0;
    int *vetorGraus;
    Vertice *vetorVertice;
    int *vetorMarcacao;
    char nome1[50];
    char nome2[50];
    long start, end;
    prim *MST;
    bool gotNegativeEdges;
    bool isUnweighted;
    dijkstra *SPT;
    char** nomesPesquisadores;

    printf("Escolha o inicio: ");
    scanf(" %[^\n]",nome1);
    printf("Escolha o final: ");
    scanf(" %[^\n]",nome2);

    printf("%s %s\n",nome1,nome2);
//    printf("%d %d\n",strlen(nome1),strlen(nome2));
    int tam = 722385;
    int inicio;  // Dijktra = 2722
    int fim;
    nomesPesquisadores = pesquisadores(tam);
    bool status1 = false;
    bool status2 = false;
    int e;
    printf("%s\n",nomesPesquisadores[11386]);
    for(e=1;e<=tam;e++){
        if(strcmp(nome1,nomesPesquisadores[e])==0){
            printf("Nome1: %s\n",nomesPesquisadores[e]);
            inicio = e;
            status1 = true;
        }
        else if(strcmp(nome2,nomesPesquisadores[e])==0){
            printf("Nome2: %s\n",nomesPesquisadores[e]);
            fim = e;
            status2 = true;
        }
        if(status1 && status2){
            break;
        }
    }

    if(!status1 || !status2){
        printf("Um dos nomes nao encontrado!");
        return;
    }
//    fim = 11386;
//    system("pause");

    // Abre um arquivo TEXTO para LEITURA
    arq = fopen("rede_colaboracao.txt", "rt");// ArqTeste   as_graph   dblp     live_journal
    if (arq == NULL)  // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return;
    }
    // Le a primeira linha para descobrir a quantidade de vertices totais
    result = fgets(Linha, NUMERO_LINHA-1, arq);
    result[strlen(result)-1] = '\0';
    numVertices = atoi(result); //transforma a string em inteiro

    vetorGraus = (int*) malloc(numVertices*sizeof(int));
    start = getMicrotime();
    vetorVertice = geraListaAdjacencia(arq,Linha,result,numVertices, &numArestas, vetorGraus,&isUnweighted,&gotNegativeEdges);
    end = getMicrotime();
        //imprimeListaAdjacencia(vetorVertice, numVertices);
    fclose(arq);
//    system("pause");
     MST = Prim(vetorVertice, numVertices, 1);
     printf("\nMST\n");
//     printMST(MST, numVertices);
     gerarArquivoPrim(MST, numVertices);

     if(!isUnweighted && !gotNegativeEdges)
     {
      printf("\nSPT\n");
      SPT = Dijkstra(vetorVertice, numVertices, inicio);
//      printDijkstra(SPT, numVertices);
      gerarArquivoDijkstra(SPT, numVertices);
//      excentricidadeDijkstra(SPT, numVertices);
//      distanciaMediaDijkstra(SPT, numVertices);
        printf("Distancia: %.2f\n",SPT->cost[fim-1]);
//      encontrarCaminho(SPT->parent,numVertices,fim);
        encontrarCaminhoPesquisador(SPT->parent,numVertices,fim,nomesPesquisadores);
//      encontrarCaminhoTodosParesVertices(SPT->parent,numVertices);
     }
     else if(isUnweighted)
      vetorMarcacao = BFSListaAdjacencia(vetorVertice, inicio, numVertices);

    printf("Tempo para gerar o Grafo: %ld\n",end - start);

//    pesquisadores();

}

