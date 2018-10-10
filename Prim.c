#include "BibliotecaTeoGraf.h"
#include "heap.h"
#include "Lista.h"
#include "Prim.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

void pesoMST(prim *d, int tam){
    int i;
    float soma = 0;
    for(i=0;i<tam;i++){
        if(d->cost[i] != FLT_MAX){
            soma+=d->cost[i];
        }
    }
    printf("O peso da MST eh de %.4f\n",soma);
}

// Funcao para gerar o arquivo com as arestas e distancias entre o vertice inicial e os demais do grafo da MST gerada por Prim
void gerarArquivoPrim(prim *d, int tam){
    FILE *arqSaida;
    int i;
    if((arqSaida = fopen("Prim.txt", "wb")) == NULL)
    {
        printf("Erro na abertura do arquivo");
        exit(1);
    }
    for(i=0;i<tam;i++){
     fprintf(arqSaida,"%d %d %.2f\r\n", d->parent[i], i+1,d->cost[i]);
    }
    fclose(arqSaida);
}

int* vizinhosMST(prim *d, int tam, int verticeNome, int *j){
    int i;
    int *vet;
    int h=0;
    for(i=0;i<tam;i++){
        if(d->parent[i] == verticeNome){
            h++;
        }else if(verticeNome == i+1){
            h++;
        }
    }

    vet = (int*) malloc(h*sizeof(int));
    for(i=0;i<tam;i++){
        if(d->parent[i] == verticeNome){
            vet[*j] = i+1;
            (*j)++;
        }else if(verticeNome == i+1 && d->parent[i] > 0){
            vet[*j] = d->parent[i];
            (*j)++;
        }
    }

    return vet;
}

void imprimeVizinhosMST(int *vet, int tam, char** nomes){
    int i;
    for(i=0;i<tam;i++){
        printf("Vizinho %d: %d %s\n",i+1,vet[i],nomes[vet[i]]);
    }
}

GrauStruct* gerarVetorGrauMST(prim *d, int tam){
    int i;
    GrauStruct* vetorGrau = (GrauStruct*) malloc(tam*sizeof(GrauStruct));
    for(i=0;i<tam;i++){
        vetorGrau[i].numGrau=0;
        vetorGrau[i].numVertice=i+1;
    }
    for(i=0;i<tam;i++){
        if(d->parent[i]>0){
            vetorGrau[i].numGrau=vetorGrau[i].numGrau+1;
            vetorGrau[d->parent[i]-1].numGrau=vetorGrau[d->parent[i]-1].numGrau + 1;
        }
    }
//    for(i=0;i<tam;i++){
//        printf("Vertice: %d -> Graus: %d\n",i+1,vetorGrau[i]);
//    }
    return vetorGrau;

}

//Essa funcao imprime o vetor de pais gerado apos executar o algoritmo de Prim
void printMST(prim *d, int n)
{
    for (int i = 0; i < n; ++i)
        printf("Aresta: %d <-> %d || peso:%lf\n", d->parent[i],i+1,d->cost[i]);
}
prim* Prim(Vertice *Grafo, int V, int start) //V eh o numero de vertices e start eh o vertice a partir do qual Prim sera executado
{
    static prim d;
    d.parent = (int*) malloc(V*sizeof(int)); //parent[i] indica qual foi o vertice que descobriu o vertice "i"
    d.cost = (float*) malloc(V*sizeof(float));//cost[i] indica o tamanho do menor caminho ate o vertice "i"
    node* aux;/*esse ponteiro guarda temporariamente posicao de memoria do novo no criado pelo malloc(sizeof(node)).Depois heap->array[i] fara esse papel*/
    bool *mstIncluded = (bool*) malloc(V*sizeof(bool));//esse vetor informa quais vertices ja foram colocados na MST
    for(int v = 0;v<V;v++)
    {
     d.parent[v]= -1;
     d.cost[v] = FLT_MAX;
     mstIncluded[v]=false;
    }
//    d.parent[start-1]= 0;
    Heap h;
    heap_init (&h,V);//inicializa o heap, isto eh, cria o vetor de "V" ponteiros para os nos (inicialmente todos NULL)
    aux = (node*)malloc(sizeof(node));//aux eh um ponteiro que aponta para os nos recem criados
    aux->vertexId = start;//cria o primeiro vertice a ser incluido na MST
    aux->currentCostToInclude = 0;//o vertice "s" possui custo zero para ser adcionado a SPT e o pai de "s" eh "-1"(nao existe)
    heap_insert(&h, aux);//insere o elemento criado no heap
    node* k = NULL;//eh um ponteiro auxiliar que guarda o vertice recem tirado do heap
    int u = -2;//u guarda o numero do vertice de origem da aresta u-v
    int v = -2;//v guarda o numero do vertice de destino da aresta u-v
    float weight = 0;//essa variavel guarda o peso de uma certa aresta no momento em que um vertice eh lido da lista de adjacencias
    Elemento *p;//ponteiro que aponta para um elemento da lista de adjacencias
    while (h.root != NULL)//Todos os vertices precisam ser incluidos na MST. Isso ocorre apos retirarmos os "V" vertices, deixando o heap vazio
    {
     k = heap_pop (&h);//retira o elemento de menor custo a ser adcionado na MST
     u = k->vertexId;//obtem o numero desse elemento
     mstIncluded[(u-1)] = true;//inclui ele na MST
     d.cost[u-1] = k->currentCostToInclude;//armazena o custo final no vetor de custos
     for(p = Grafo[u-1].adjancencia; p!= NULL; p=proximaLista(p))/*Obtem todos os elementos adjacentes a ele e verifica um a um se precisa um custo menor foi encontrado*/
     {
      v = getItem(p);//obtem o numero do elemento adjacente
      weight = getWeight(p);//obtem o peso da aresta para incluir o elemento
      if(!h.array[v-1])//checa se o elemento v ja foi inserido no heap, isto eh, se h.array[v-1]==NULL
      {
       aux = (node*)malloc(sizeof(node));//Se nao foi colocado no heap entao o no eh criado
       aux->vertexId = v;//O id do novo vertice a ser incluido eh o proprio "v"
       aux->currentCostToInclude = weight;/*Inicializa a distancia ate ele na primeira vez que foi encontrado com o peso da aresta*/
       heap_insert(&h, aux);//insere o elemento criado no heap
      }
      else if( (mstIncluded[v-1] == false) && (h.array[v-1]->currentCostToInclude > weight) )/*Checa se o vertice ja foi incluido ou nao na MST e se o novo custo pra incluir eh menor que o atual*/
      {
       d.parent[v-1] = u;//atualiza o vetor de pais
       heap_decrease (&h, (v-1), weight);//muda o valor da chave do no "v" com o novo peso
      }
     }
    }
  //printMST(parent, V);
  return &d;
}

// Merge Sort para um vetor com a struct "GrauStruct"
void mergeStructGrauStruct(GrauStruct arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    GrauStruct *L = (GrauStruct*) malloc(n1*sizeof(GrauStruct));
    GrauStruct *R = (GrauStruct*) malloc(n2*sizeof(GrauStruct));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i].numGrau <= R[j].numGrau)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortGrauStruct(GrauStruct *arr, int l, int r)
{
    if (l < r)
    {
        int m = l+(r-l)/2;

        mergeSortGrauStruct(arr, l, m);
        mergeSortGrauStruct(arr, m+1, r);

        mergeStructGrauStruct(arr, l, m, r);
    }
}

