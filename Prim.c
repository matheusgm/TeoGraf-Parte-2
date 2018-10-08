#include "BibliotecaTeoGraf.h"
#include "heap.h"
#include "Lista.h"
#include "Prim.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    bool* mstIncluded = (bool*) malloc(V*sizeof(bool));//esse vetor informa quais vertices ja foram colocados na MST
    int vv;
    for(vv = 0;vv<V;vv++)
    {
     d.parent[vv]= -1;
     d.cost[vv] = -1;
     mstIncluded[vv]=false;
    }
    Heap h;
    heap_init (&h,V);//inicializa o heap, isto eh, cria todos os "V" nos com custo FLT_MAX
    d.parent[(start-1)] == 0;//O custo para adcionar o vertice de comeco na MST eh zero
    heap_decrease (&h, (start-1), 0);//o vertice "s" possui custo zero para ser adcionado a MST e o pai de "s" eh "-1"(nao existe)
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
      if( (mstIncluded[v-1] == false) && (h.array[v-1]->currentCostToInclude > weight) )/*Checa se o vertice ja foi incluido ou nao na MST e se o novo custo pra incluir eh menor que o atual*/
      {
       d.parent[v-1] = u;//atualiza o vetor de pais
       heap_decrease (&h, (v-1), weight);//muda o valor da chave do no "v" com o novo peso
      }

     }
    }
  //printMST(parent, V);
  return &d;
}

