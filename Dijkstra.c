#include "BibliotecaTeoGraf.h"
#include "heap.h"
#include "Lista.h"
#include "PilhaLista.h"
#include "Dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void distanciaMediaDijkstra(dijkstra* d, int tam){
    int i;
    int total = 0;
    float soma = 0;
    for(i=0;i<tam;i++){
        if(d->cost[i] != -1){
            soma+=d->cost[i];
            total++;
        }
    }
    float media = soma/total;
    printf("A distancia media do grafo eh de %.2f\n",media);
}

void encontrarCaminho(int *vetPai, int tam, int s){
    int x = vetPai[s-1];
    Pilha *pilha = pilha_cria();
    pilha_push(pilha, s);
    while(x!=-1){
        pilha_push(pilha, x);
        x = vetPai[x-1];
    }
    printf("Caminho: ");
    while(!pilha_vazia(pilha)){
        int num = pilha_pop(pilha);
        if(num != s){
            printf("%d -> ",num);
        }else{
            printf("%d\n",num);
        }
    }
}

void encontrarCaminhoPesquisador(int *vetPai, int tam, int s, char** nomePesquisador){
    int x = vetPai[s-1];
    Pilha *pilha = pilha_cria();
    pilha_push(pilha, s);
    while(x!=-1){
        pilha_push(pilha, x);
        x = vetPai[x-1];
    }
    printf("Caminho: ");
    while(!pilha_vazia(pilha)){
        int num = pilha_pop(pilha);
        if(num != s){
            printf("%s -> ",nomePesquisador[num]);
        }else{
            printf("%s\n",nomePesquisador[num]);
        }
    }
}

void encontrarCaminhoTodosParesVertices(int *vetPai, int tam){
    int i;
    for(i=0;i<tam;i++){
        printf("Vertice %d: ",i+1);
        encontrarCaminho(vetPai,tam,i+1);
    }
}

void excentricidadeDijkstra(dijkstra* d, int tam){
    int i;
    float distanciaMaior = d->cost[0];
    int verticeMaior = 1;
    //system("pause");
    for(i=1;i<tam;i++){
        if(d->cost[i] > distanciaMaior){
            distanciaMaior = d->cost[i];
            verticeMaior = i+1;
        }
    }
    printf("Excentricidade: %f -> para o vertice %d\n",distanciaMaior,verticeMaior);
}

// Funcao para gerar o arquivo com as arestas e distancias entre o vertice inicial e os demais do grafo da SPT gerada por Dijkstra
void gerarArquivoDijkstra(dijkstra *d, int tam){
    FILE *arqSaida;
    int i;
    if((arqSaida = fopen("Dijkstra.txt", "wb")) == NULL)
    {
        printf("Erro na abertura do arquivo");
        exit(1);
    }
    for(i=0;i<tam;i++){
     fprintf(arqSaida,"Aresta: %d <-> %d Menor distancia ateh %d: %.2f\r\n", d->parent[i], i+1,i+1,d->cost[i]);
    }
    fclose(arqSaida);
}

//Essa funcao imprime o vetor de pais gerado apos executar o algoritmo de Dijkstra, tal como as distancias
void printDijkstra(dijkstra *d, int n)
{
    for (int i = 0; i < n; ++i)
        printf("Caminho: %d <-> %d || Menor caminho ateh %d:%lf\n", d->parent[i], i+1,i+1,d->cost[i]);
}

dijkstra* Dijkstra(Vertice *Grafo, int V, int start) //V eh o numero de vertices e start eh o vertice a partir do qual Prim sera executado
{
    static dijkstra d;
    d.parent = (int*) malloc(V*sizeof(int)); //parent[i] indica qual foi o vertice que descobriu o vertice "i"
    d.cost = (float*) malloc(V*sizeof(float));//cost[i] indica o tamanho do menor caminho ate o vertice "i"
    bool *sptIncluded = (bool*) malloc(V*sizeof(bool));//esse vetor informa quais vertices ja foram colocados no SPT(shortest path tree)
    int vv;
    for( vv = 0;vv<V;vv++)
    {
     d.parent[vv]= -1;
     d.cost[vv]= -1;
     sptIncluded[vv]=false;
    }
    Heap h;
    heap_init (&h,V);//inicializa o heap, isto eh, cria todos os "V" nos com custo FLT_MAX
    d.parent[(start-1)] == 0;//O custo para adcionar o vertice de comeco na SPT eh zero
    heap_decrease (&h, (start-1), 0);//o vertice "s" possui custo zero para ser adcionado a SPT e o pai de "s" eh "-1"(nao existe)
    node* k = NULL;//eh um ponteiro auxiliar que guarda o vertice recem tirado do heap
    int u = -2;//u guarda o numero do vertice de origem da aresta u-v
    int v = -2;//v guarda o numero do vertice de destino da aresta u-v
    float weight = 0;//essa variavel guarda o peso de uma certa aresta no momento em que um vertice eh lido da lista de adjacencias
    float newCost;//variavel que armazena o custo para se adcionar um vertice
    Elemento *p;//ponteiro que aponta para um elemento da lista de adjacencias
    while (h.root != NULL)//Todos os vertices precisam ser incluidos na SPT. Isso ocorre apos retirarmos os "V" vertices, deixando o heap vazio
    {
     k = heap_pop (&h);//retira o elemento de menor custo a ser adcionado na SPT
     u = k->vertexId;//obtem o numero desse elemento
     sptIncluded[(u - 1)] = true;//inclui ele na SPT
     d.cost[u-1] = k->currentCostToInclude;
     //printf("Menor caminho ateh %d:%.2f\n",u,d.cost[u-1]);
     for(p = Grafo[u-1].adjancencia; p!= NULL; p=proximaLista(p))/*Obtem todos os elementos adjacentes a ele e verifica um a um se precisa um custo menor foi encontrado*/
     {
      v = getItem(p);//obtem o numero do elemento adjacente
      weight = getWeight(p);//obtem o peso da aresta para incluir o elemento
      newCost = (weight + h.array[(u-1)]->currentCostToInclude);
      if( (sptIncluded[(v-1)] == false) && (h.array[(v-1)]->currentCostToInclude > newCost) )/*Checa se o vertice ja foi incluido ou nao na SPT e se o novo custo pra incluir eh menor que o atual*/
      {
          //system("pause");
       d.parent[(v-1)] = u;//atualiza o vetor de pais
       heap_decrease (&h, (v-1), newCost);//muda o valor da chave do no "v" com o novo custo
      }

     }
    }
  //printMST(parent, V);
  return &d;
}
