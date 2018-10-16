#ifndef _HEAP_H
#define _HEAP_H
void encontrarCaminhoPesquisador(int *vetPai, int tam, int s, char** nomePesquisador);
typedef struct node
{
 float currentCostToInclude;//Contem o custo atual para incluir na MST
 int vertexId;//Esse campo contem o id do vertice de destino de uma aresta
 struct node* prev;/*esse ponteiro pode ser tanto o anterior da lista duplamente encadeada quanto um ponteiro pra o pai se o no for o primeiro da lista*/
 struct node* next;/*esse ponteiro aponta pro proximo da lista duplamente encadeada*/
 struct node* child;/*esse ponteiro aponta para o no que comeca uma lista duplamente encadeada dos filhos*/
}node;


/*Essa estrutura eh uma estrutura container, de forma similar as classes containers da linguagem C++. Nela guardamos quantos nos existem, tal como um ponteiro que aponta para o topo da arvore, isto eh o elemento cuja chave eh a menor dentre todas do heap. Alem disso tem um vetor que guarda os pais dos nos da MST e outro vetor que guarda a posicao de cada no dentro do heap*/
typedef struct Heap
{
 unsigned int size;//diz quantos nos estao incluidos no heap
 node *root;//aponta para o elemento cuja chave eh minima
 node** array;//esse vetor guarda a posicao de memoria de cada um dos elementos do heap. Util quando quisermos diminuir sua respectiva chave
}Heap;

void heap_init (Heap *heap,int V);//inicializa todos os elementos do heap com custo INT_MAX e o vetor de pais "-1"

void heap_insert (Heap *heap,node *elem);//insere um elemento no heap

node *heap_pop (Heap *heap);//retira o primeiro no de chave minima

void
heap_decrease (Heap *heap, int destinationVertexId, float newValue);/*essa funcao eh chamada sempre que um novo valor eh atualizado*/


#endif /* _HEAP_H */
