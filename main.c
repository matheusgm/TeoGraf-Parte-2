#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Lista.h"
#include "FilaLista.h"
#include "PilhaLista.h"
#include "FuncoesAuxiliares.h"
#include "BibliotecaTeoGraf.h"
#include "Prim.h"
#include "Dijkstra.h"
#include <locale.h>
#define NUMERO_LINHA 1000


int main()
{
    /*
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
    long start, end;
    prim *MST;
    bool gotNegativeEdges;
    bool isUnweighted;
    dijkstra *SPT;
    //printf("Escolha o arquivo: ['as_graph.txt','dblp.txt','live_journal.txt']\n");
    //scanf(" %s",nomeArq);
    //printf("\nNome do arquivo escolhido:%s-\n",%s);

    // Abre um arquivo TEXTO para LEITURA
    arq = fopen("grafo_1.txt", "rt");// ArqTeste   as_graph   dblp     live_journal
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
     int inicio = 1;  // Dijktra = 2722
     int fim =2;
     if(!isUnweighted && !gotNegativeEdges)
     {
      printf("\nSPT\n");
      SPT = Dijkstra(vetorVertice, numVertices, inicio);
      printDijkstra(SPT, numVertices);
      gerarArquivoDijkstra(SPT, numVertices);
      excentricidadeDijkstra(SPT, numVertices);
      distanciaMediaDijkstra(SPT, numVertices);
        printf("Distancia: %.2f\n",SPT->cost[fim-1]);
      encontrarCaminho(SPT->parent,numVertices,fim);
//      encontrarCaminhoTodosParesVertices(SPT->parent,numVertices);
     }
     else if(isUnweighted)
      vetorMarcacao = BFSListaAdjacencia(vetorVertice, inicio, numVertices);

//      vetorMarcacao = DFSListaAdjacencia(vetorVertice, 1, numVertices);

//    componenteConexa(vetorVertice,numVertices);

//    milCiclosListaAdjacencia(vetorVertice, numVertices, "BFS_ListaAdjacencia.txt",0); // 0(BFS) ou 1(DFS)

//    int o;
//    o = diametroGrafo(vetorVertice, numVertices);
//    printf("Diametro: %d\n",o);

    // Calcula os Graus e gera o arquivo de Saida
//    gerarGraus(vetorGraus, numVertices,numArestas,"Saida.txt");

    printf("Tempo para gerar o Grafo: %ld\n",end - start);

//    pesquisadores();

    */

    //setlocale(LC_ALL, "Portuguese");
    redeColaboracao();

    return 1;
}




