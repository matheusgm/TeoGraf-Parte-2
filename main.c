#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Lista.h"
#include "FilaLista.h"
#include "PilhaLista.h"
#include "FuncoesAuxiliares.h"
#include "BibliotecaTeoGraf.h"
#include "Prim.h"
#include "Dijkstra.h"
#include <sys/time.h>

//double time_diff(struct timeval x , struct timeval y);
#define NUMERO_LINHA 1000


int main()
{
    FILE *arq;
    char Linha[NUMERO_LINHA];
    char *result;
    int numVertices;
    int numArestas = 0;
    int *vetorGraus;
    Vertice *vetorVertice;
    char *MatrizVertice;
    int *vetorMarcacao;
    char escolha;
    char nomeArq[50];
    prim *MST;
    bool gotNegativeEdges;
    bool isUnweighted;
    dijkstra *SPT;
    int start;//vertice de inicio dos algoritmos de Dijjkstra e Prim
    double acumuladorTempo = 0;
    struct timeval before , after;
    srand(time(NULL));//gera a semente da funcao rand
    //struct timespec comeco, fim;
    // Abre um arquivo TEXTO para LEITURA
    arq = fopen("grafo_1.txt", "rt");
    if (arq == NULL)  // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }

    // Le a primeira linha para descobrir a quantidade de vertices totais
    result = fgets(Linha, NUMERO_LINHA-1, arq);
    result[strlen(result)-1] = '\0';
    numVertices = atoi(result); //transforma a string em inteiro

    vetorGraus = (int*) malloc(numVertices*sizeof(int));
    //start = getMicrotime();
    vetorVertice = geraListaAdjacencia(arq,Linha,result,numVertices, &numArestas, vetorGraus,&isUnweighted,&gotNegativeEdges);
    //end = getMicrotime();
        //imprimeListaAdjacencia(vetorVertice, numVertices);

    fclose(arq);
    //system("pause");

     MST = Prim(vetorVertice, numVertices,1);
     printf("\nMST\n");
     //printMST(MST, numVertices);
     pesoMST(MST, numVertices);
     gerarArquivoPrim(MST, numVertices);

     free(MST->cost);
     free(MST->parent);
     if(!isUnweighted && !gotNegativeEdges)
     {
      printf("\nSPT\n");
      SPT = Dijkstra(vetorVertice, numVertices,1);
      //printDijkstra(SPT, numVertices);
      //gerarArquivoDijkstra(SPT, numVertices);
      excentricidadeDijkstra(SPT,numVertices,1);
      //distanciaMediaDijkstra(vetorVertice, numVertices);
     }
     else if(isUnweighted)
      vetorMarcacao = BFSListaAdjacencia(vetorVertice, 1, numVertices);
   free(SPT->cost);
   free(SPT->parent);

   redeColaboracao(); // Edsger W. Dijkstra - Daniel R. Figueiredo
   /*for(int i = 0; i<10; i++)

   {

    start = (rand()%numVertices);

    gettimeofday(&before , NULL);

    MST = Prim(vetorVertice, numVertices, start);

    pesoMST(MST, numVertices);

    gettimeofday(&after , NULL);

    acumuladorTempo +=time_diff(before , after);

    free(MST->cost);

    free(MST->parent);

   }

   printf("\nTempo medio MST:%.0f\n",(double)acumuladorTempo/100);

   acumuladorTempo = 0;
   for(int i = 0; i<10; i++)
   {
    start = (rand()%numVertices);
    gettimeofday(&before , NULL);
    SPT = Dijkstra(vetorVertice, numVertices, start);
    excentricidadeDijkstra(SPT,numVertices,start);
    gettimeofday(&after , NULL);
    acumuladorTempo +=time_diff(before , after);
    free(SPT->cost);
    free(SPT->parent);
   }
   printf("\nTempo medio excentricidade:%.0f\n",(double)acumuladorTempo/100);
   acumuladorTempo=0;


    gettimeofday(&before , NULL);
    distanciaMediaDijkstra(vetorVertice, numVertices);
    gettimeofday(&after , NULL);
    acumuladorTempo +=time_diff(before , after);
    printf("\nTempo medio distancia media:%.2lf\n",acumuladorTempo);
    acumuladorTempo = 0;
    */
//      vetorMarcacao = DFSListaAdjacencia(vetorVertice, 1, numVertices);

//    componenteConexa(vetorVertice,numVertices);

//    milCiclosListaAdjacencia(vetorVertice, numVertices, "BFS_ListaAdjacencia.txt",0); // 0(BFS) ou 1(DFS)

//    int o;
//    o = diametroGrafo(vetorVertice, numVertices);
//    printf("Diametro: %d\n",o);

    // Calcula os Graus e gera o arquivo de Saida
//    gerarGraus(vetorGraus, numVertices,numArestas,"Saida.txt");

    //printf("Tempo para gerar o Grafo: %ld\n",end - start);
    return 1;
}
/*
double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;

    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

    diff = (double)y_ms - (double)x_ms;

    return diff;
}*/

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
    GrauStruct* grausMST;

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
//    printf("%s\n",nomesPesquisadores[11386]);
    for(e=1;e<=tam;e++){
        if(strcmp(nome1,nomesPesquisadores[e])==0){
            printf("Nome1: %s\n",nomesPesquisadores[e]);
            inicio = e;
            status1 = true;GrauStruct* grausMST;
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
     int jj =0;
     int* lstDijkstra = vizinhosMST(MST,numVertices,2722,&jj);
     printf("Vizinhos de Dijkstra: \n");
     imprimeVizinhosMST(lstDijkstra,jj,nomesPesquisadores);
     jj =0;
     int* lstDaniel = vizinhosMST(MST,numVertices,343930,&jj);
     printf("Vizinhos de Daniel: \n");
     imprimeVizinhosMST(lstDaniel,jj,nomesPesquisadores);

     grausMST = gerarVetorGrauMST(MST,numVertices);
     mergeSortGrauStruct(grausMST,0,numVertices-1);
     printf("Maior Grau 1: %d -> Vertice: %d -> %s\n",grausMST[numVertices-1].numGrau,grausMST[numVertices-1].numVertice,nomesPesquisadores[grausMST[numVertices-1].numVertice]);
     printf("Maior Grau 2: %d -> Vertice: %d -> %s\n",grausMST[numVertices-2].numGrau,grausMST[numVertices-2].numVertice,nomesPesquisadores[grausMST[numVertices-2].numVertice]);
     printf("Maior Grau 3: %d -> Vertice: %d -> %s\n",grausMST[numVertices-3].numGrau,grausMST[numVertices-3].numVertice,nomesPesquisadores[grausMST[numVertices-3].numVertice]);



    // Edsger W. Dijkstra - Daniel R. Figueiredo

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



