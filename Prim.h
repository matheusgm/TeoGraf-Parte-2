#include "Lista.h"
#include "heap.h"
typedef struct PrimCostAndParent
{
 int *parent;
 float *cost;
}prim;

typedef struct grauStruct GrauStruct;
struct grauStruct{
    int numGrau;
    int numVertice;
};

void mergeSortGrauStruct(GrauStruct *arr, int l, int r);
int* vizinhosMST(prim *d, int tam, int verticeNome, int *j);
void imprimeVizinhosMST(int *vet, int tam, char** nomes);
GrauStruct* gerarVetorGrauMST(prim *d, int tam);
void pesoMST(prim *d, int tam);
void gerarArquivoPrim(prim *d, int tam);
prim* Prim(Vertice *Grafo, int V, int start);/*Essa funcao retorna um vetor de pais da MST para uma quantidade V de vertices comecando pelo vertice start*/
void printMST(prim *d, int n);//Essa funcao imprime as arestas incluidas na MST a partir do vetor de pais "arr[]"
