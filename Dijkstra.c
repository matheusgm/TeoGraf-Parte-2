#include "BibliotecaTeoGraf.h"
#include "heap.h"
#include "Lista.h"
#include "PilhaLista.h"
#include "Dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <limits.h>

void distanciaMediaDijkstra(Vertice* Grafo, int tam){
    int i,j;
    int combinacoesValidas = 0;
    float soma = 0;
    dijkstra*d;//armazena o resultado de dijkstra
    for(i=1;i<tam;i++)
    {
     d = Dijkstra(Grafo, tam, i);
     for(j = i+1;j<=tam;j++)/*j = i retira as redundancias, isto eh, dist(u,v) = dist(v,u). Alem disso nao precisa contar os zeros. Esse loop interno faz a soma de todas as distancias para um dado dijkstra que comecou no vertice "i"*/
     {
      if(d->cost[j] != FLT_MAX)
      {
       soma += d->cost[j];
       combinacoesValidas++;
       //printf("Distancia: %d <-> %d\r\n",i,j);
      }
     }
     //printf("\nI:%d\n",i);
     free(d->cost);
     free(d->parent);
    }
    //printf("A distancia media do grafo eh de %.4f\n",(soma/combinacoesValidas));
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


void excentricidadeDijkstra(dijkstra *d, int tam, int s){
    int i;
    float distanciaMaior = d->cost[0];
    int verticeMaior = 1;
    for(i=1;i<tam;i++){
        if(d->cost[i] > distanciaMaior){
            distanciaMaior = d->cost[i];
            verticeMaior = i+1;
        }
    }
    //printf("Maior distancia do vertice %d eh de %.4f para o vertice %d\n",s,distanciaMaior,verticeMaior);
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
     fprintf(arqSaida,"Aresta: %d <-> %d Menor distancia ateh %d: %.4lf\r\n", d->parent[i], i+1,i+1,d->cost[i]);
    }
    fclose(arqSaida);
}

//Essa funcao imprime o vetor de pais gerado apos executar o algoritmo de Dijkstra, tal como as distancias
void printDijkstra(dijkstra *d, int n)
{
    for (int i = 0; i < n; ++i)
        printf("Caminho: %d <-> %d || Menor caminho ateh %d:%lf\n", d->parent[i], i+1,i+1,d->cost[i]);
}

dijkstra* Dijkstra(Vertice *Grafo, int V, int start) /*V eh o numero de vertices e start eh o vertice a partir do qual Dijkstra sera executado.*/
{
    static dijkstra d;
    d.parent = (int*) calloc(V,sizeof(int)); //parent[i] indica qual foi o vertice que descobriu o vertice "i"
    d.cost = (float*) calloc(V,sizeof(float));//cost[i] indica o tamanho do menor caminho ate o vertice "i"
    node* aux;/*esse ponteiro guarda temporariamente posicao de memoria do novo no criado pelo malloc(sizeof(node)).Depois heap->array[i] fara esse papel*/
    bool *sptIncluded = (bool*) calloc(V,sizeof(bool));//esse vetor informa quais vertices ja foram colocados no SPT(shortest path tree)
    for(int v = 0;v<V;v++)
    {
     d.parent[v]= -1;
     d.cost[v]= FLT_MAX;
     sptIncluded[v]=false;
    }
    Heap h;
    heap_init (&h,V);//inicializa o heap, isto eh, cria o vetor de "V" ponteiros para os nos (inicialmente todos NULL)
    aux = (node*)malloc(sizeof(node));//aux eh um ponteiro que aponta para os nos recem criados
    aux->vertexId = start;//cria o primeiro vertice a ser incluido na SPT
    aux->currentCostToInclude = 0;//o vertice "s" possui custo zero para ser adcionado a SPT e o pai de "s" eh "-1"(nao existe)
    heap_insert(&h, aux);//insere o elemento criado no heap
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
     d.cost[u-1] = k->currentCostToInclude;//Salva no vetor de custo o custo minimo
     for(p = Grafo[u-1].adjancencia; p!= NULL; p=proximaLista(p))/*Obtem todos os elementos adjacentes a ele e verifica um a um se um custo menor foi encontrado*/
     {
      v = getItem(p);//obtem o numero do elemento adjacente
      weight = getWeight(p);//obtem o peso da aresta para incluir o elemento
      if(!h.array[v-1])//checa se o elemento v ja foi inserido no heap, isto eh, se h.array[v-1]==NULL
      {
       d.parent[(v-1)] = u;
       aux = (node*)malloc(sizeof(node));//Se nao foi colocado no heap entao o no eh criado
       aux->vertexId = v;//O id do novo vertice a ser incluido eh o proprio "v"
       aux->currentCostToInclude = weight + h.array[(u-1)]->currentCostToInclude;/*Inicializa a distancia ate ele na primeira vez que foi encontrado com o peso da aresta + a distancia atual ate o pai*/
       heap_insert(&h, aux);//insere o elemento criado no heap
      }
      else
      {
       newCost = (weight + h.array[(u-1)]->currentCostToInclude);
       if( (sptIncluded[(v-1)] == false) && (h.array[(v-1)]->currentCostToInclude > newCost) )/*Checa se o vertice ja foi incluido ou nao na SPT e se o novo custo pra incluir eh menor que o atual*/
       {
        d.parent[(v-1)] = u;//atualiza o vetor de pais
        heap_decrease (&h, (v-1), newCost);//muda o valor da chave do no "v" com o novo custo
       }
      }
     }
    }
  //printMST(parent, V);
  return &d;
}
