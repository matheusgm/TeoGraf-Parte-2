#include <stdio.h>
#include <stdbool.h>
#include "typeVertice.h"
typedef struct arvore Arvore;
typedef struct marcacao2 Marcacao2;
typedef struct endVetorPrincipal EndVetorPrincipal;

// ----------------------------------------------------- INICIO DE GERAR GRAFOS -----------------------------------------------------
Vertice* geraListaAdjacencia(FILE *arq,char *Linha,char *result, int numVertices, int *numArestas, int *vetorGraus,bool*isUnweighted,bool*gotNegativeEdges);
// ----------------------------------------------------- FIM DE GERAR GRAFOS -----------------------------------------------------

// ----------------------------------------------------- INICIO DE IMPRIMIR GRAFOS -----------------------------------------------------
void imprimeMatrizBYTE(int** MatrizVertice, int numVertices);

// Imprime o Grafo na forma de Lista de Adjacencia
void imprimeListaAdjacencia(Vertice *vetorVertice, int numVertices);
// ----------------------------------------------------- FIM DE IMPRIMIR GRAFOS -----------------------------------------------------

// ----------------------------------------------------- INICIO ARQUIVOS DE SAIDA -----------------------------------------------------
// Funcao para gerar o arquivo de Arvore
void gerarArquivoArvore(Arvore *arvore, int tam);

// Gera um Arquivo de Saida com o Numero Total de Vertices, Numero Total de Arestas, Grau Maximo, Grau Minimo, Grau Medio e Mediana dos Graus
void gerarArquivoSaida(int numeroVertice, int numArestas, int gMax, int gMin, float gMediana, float gMedio, char* nomeArqSaida);
// ----------------------------------------------------- FIM ARQUIVOS DE SAIDA -----------------------------------------------------

// ----------------------------------------------------- INICIO DE BUSCA -----------------------------------------------------
// BFS de Lista de Adjacencia
int* BFSListaAdjacencia(Vertice *Grafo, int s, int tam);

// DFS de Lista de Adjacencia
int* DFSListaAdjacencia(Vertice *Grafo, int s, int tam);

// BFS de Lista de Adjacencia para utilizar na busca de Componentes Conexos
void BFSListaAdjacencia02(Vertice *Grafo, int s, int tam, Marcacao2* vetorMarcacao, int numMarcado);
// ----------------------------------------------------- FIM DE BUSCA -----------------------------------------------------

// ----------------------------------------------------- INICIO CICLO 1000X -----------------------------------------------------
// Executa, para Lista Adjacencia, a BFS(0) ou DFS(1) 1000x exibindo o tempo media e gerando um arquivo com o tempo em cada ciclo
void milCiclosListaAdjacencia(Vertice *vetorVertice, int numVertices, char* nomeArquivo, int tipo);

// ----------------------------------------------------- FIM CICLO 1000X -----------------------------------------------------

// ----------------------------------------------------- OUTRAS FUNÇÕES -----------------------------------------------------
void componenteConexa(Vertice *Grafo, int tam);

void gerarGraus(int* vetorGraus, int numVertices, int numArestas, char* nomeArqSaida);

int diametroGrafo(Vertice *Grafo, int tam );


