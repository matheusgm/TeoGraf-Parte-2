#include "Lista.h"
#include "heap.h"
typedef struct dijkstraCostAndParent
{
 int *parent;
 float *cost;
}dijkstra;

void distanciaMediaDijkstra(Vertice* Grafo, int tam);/*funcao que calcula a distancia media, desconsiderando distancias infinitas*/
void excentricidadeDijkstra(dijkstra *d,int tam,int s);/*Calcula a maior distancia entre o vertice de origem s e qualquer outro do grafo*/
void gerarArquivoDijkstra(dijkstra *d, int tam);//Funcao para gerar o arquivo com as arestas e distancias entre o vertice inicial e os demais do grafo da SPT gerada por Dijkstra
dijkstra* Dijkstra(Vertice *Grafo, int V, int start);/*Essa funcao retorna um vetor de pais da SPT para uma quantidade V de vertices comecando pelo vertice start*/
void printDijkstra(dijkstra *d, int n);//Essa funcao imprime as arestas incluidas na SPT bem como o custo  
