#include "heap.h"
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Essa funcao efetua o merge entre dois heaps. Para tal, ela avalia apenas o primeiro elemento (a raiz) de cada heap. No final, aquele que tiver a maior chave se tornara filho (um subheap) do que tiver a menor chave. Em seguida, a funcao retorna um ponteiro para a raiz do novo heap.*/
static node *
merge (node *a, node *b)
{
  if (a->currentCostToInclude < b->currentCostToInclude) //Retorna true caso a chave do elemento a seja menor que a chave do elemento b
    {
      if (a->child != NULL)
        a->child->prev = b;
/*Suponha a seguinte configuracao antes da linha acima ser executa, com chave A < chave B, onde "--->" representam ponteiros:
 *
 *  A 
 * |^
 * ||
 * ^| 
 * W ---> X ---> Y     B ---> K
 *   <---   <---         <---
 *
 *Sendo A e B as respectivas raizes de seus heaps. Apos a linha anterior ser executada teremos: 
 *                 A 
 *                 |
 *                 |
 *                 ^  
 * k  <--- B  <--- W ---> X ---> Y     
 *    --->           <---   <---   
 *
 * Isto eh, A continua apontando para W, no entanto o ponteiro anterior de W passa a apontar para B   
 * */
      if (b->next != NULL)
        b->next->prev = a;
/*  
 *Continuando com o exemplo:
 *     ----------->A 
 *    /            |
 *   /             |
 *  /              ^  
 * k  <--- B  <--- W ---> X ---> Y     
 *                   <---   <---   
 */    
      a->next = b->next;
/*  
 *     ---------->A ---> k
 *    /           |
 *   /            |
 *  /             ^  
 * k  <--- B <--- W ---> X ---> Y     
 *                  <---   <---   
 * 
 * (k aqui eh representado duas vezes para facilitar)     
 */    
      b->next = a->child;
/*  
 *               A ---> k
 *               | <---
 *               |
 *               ^  
 *        B <--- W ---> X ---> Y     
 *          --->   <---   <---   
 */    
         
     a->child = b;
/*  
 *               A ---> k
 *               | <---
 *               |
 *               ^  
 *               B <--- W ---> X ---> Y     
 *                  --->   <---   <---   
 */    
      b->prev = a;
/*  
 *                A ---> k
 *               ^| <---
 *               ||
 *               |^  
 *               B <--- W ---> X ---> Y     
 *                  --->   <---   <---   
 */    
      return a;
    }
  if (b->child != NULL)//Caso simetrico, isto eh, quando b <= a
    b->child->prev = a;
  if (a->prev != NULL && a->prev->child != a)
    a->prev->next = b;
  if(a->prev)
   b->prev = a->prev;
  else
   b->prev = NULL;
  a->prev = b;
  if(b->child)
   a->next = b->child;
  else
   a->next = NULL;
  b->child = a;
  return b;
}

/* Insere um elemento no heap. Para tanto avalia apenas a chave da raiz e a chave do novo elemento a ser adcionado. A posicao do novo elemento no heap sera determinada pela funcao merge apresentada anteriormente*/
void
heap_insert (Heap *heap,node *elem)
{ 
  heap->array[(elem->vertexId - 1)] = elem;/*Array[i] contem o endereco de memoria do no recem inserido no heap. Assim sendo, quando criamos um novo elemento e o inserimos no heap, eh necessario saber qual o numero identificador(k) dele para que o k-esimo - 1 ponteiro do array aponte para o no que esta sendo inserido*/ 
  heap->size++;//incrementa o tamanho do heap
  elem->prev = NULL;
  elem->next = NULL;
  elem->child = NULL; 
  /*Inicialmente todos os ponteiros do novo elemento sao inicializados com NULL*/
  if (heap->root == NULL)
  {
   heap->root = elem;//Caso o heap esteja vazio, entao a raiz apontara para o novo elemento
  }
  
/*Na primeira vez que um elemento for inserido no heap teremos essa configuracao:
 *
 *heap            node a         
 {               {
  size = 1;       currentCostToInclude = (valor da chave do elemento que esta sendo inserido);  
  array[0] == &a;   
  root-----------> 
 *}                 child = NULL; 
 *                 prev = NULL;
 *                 next = NULL;
 *                }
 *               }
 *Root contem o endereco para o no "a" e array[i] contem o endereco de memoria do no cujo vertexId eh "i"      
*/  
  else
   heap->root = merge (elem, heap->root);//Caso o heap nao esteja vazio, eh necessario efetuar um merge entre a raiz e o novo elemento 
  
/*Suponha que ja tenha um elemento inserido no heap. Supondo que o valor da chave "a" < valor da chave "b", apos a nova insercao teremos a seguinte configuracao:
 *
 *heap            node a                                       node b
 {               {                                             {
  size = 1;       currentCostToInclude = (valor da chave a);    currentCostToInclude = (valor da chave b);
  root== &a
  array[0] == &a
  array[1] == &b
 *}                child == &b
 *                 prev = NULL;                                               
 *                 next = NULL;                                 child = NULL;   
 *                                                              prev == &a 
 *               }                                              next = NULL;
 *                                              
 *                                                             }   
 *Root contem o endereco para o no "a" e array[i] contem o endereco de memoria do no cujo vertexId eh "i"        
*/  
}

/* Essa funcao recebe como input o ponteiro child do no raiz. Apos remover a raiz, teremos uma floresta formada pelas suas subarvores, que nada mais eh do que uma lista duplamente encadeada comecando pelo filho mais a esquerda, que era apontado pelo ponteiro child do no raiz. Essa funcao efetua o merge dois a dois entre as subarvores indo da esquerda para a direita. No final teremos ainda uma floresta resultante dos merges. Um ponteiro para a ultima arvore resultante do merge eh retornado */
static node *
merge_right (node *a)
{
  node *b;
/* Se antes tinhamos, por exemplo, a seguinte configuracao apos remover o no raiz:
 *(RAIZ -> child == A)
 *RAIZ
 *
 * A ---> B ---> C  ---> D  ---> E
 *   <---   <---    <---    <---
 * |^     |^     |^      |^      |^
 * ||     ||     ||      ||      ||
 * ^|     ^|     ^|      ^|      ^|
 * Ta     Tb     Tc      Td      Te
 *
 * Onde Ta,Tb... representam respectivamente as subarvores dos nos A,B... 
 * */
  for (b = NULL; a != NULL; a = b->next)
    {
      if ((b = a->next) == NULL)
        return a;
      b = merge (a, b);
    }
/*

1 iteracao: b == &B || a == &A
2 iteracao: a == &C || b == &D
3 iteracao: a == &E || b == NULL
*/


/*
 *Supondo que a chave do no A < chave do no B e chave do no D < chave do no C, apos a execucao acima teremos:
 *
 *          A  -----------------------> D  --------------> E
 *          |^ <----------------------- |^ <-------------- |^
 *          ||                          ||                 ||
 *          ^|                          ^|                 ^|
 *          B  ---> Ta                  C ---> Td          Te
 *          |^ <---                     |^ <--- 
 *          ||                          ||
 *          ^|                          ^|
 *          Tb                          Tc
 *
 * Primeiramente os heaps A e B foram passados pro merge. Em seguida D e C. Nesse momento "a" aponta pra E e portanto a->next == NULL. A funcao retorna o ponteiro para E
 *
 * Note que apos a execucao ainda nao obtemos uma arvore unica. Assim, de forma a obtermos um unico heap e por conseguinte a raiz, novas operacaoes de merge precisam ser executadas. Dessa vez comecando pela direita, efetuaremos novos merges com os heaps (arvores) consecutivos e nao em separado tal como foi feito acima na funcao merge_left 
 * */       

  return b;
}

/* Essa funcao efetua a segunda sequencia de merges indo da direita pra esquerda. Essa funcao recebe um ponteiro para o ultimo heap resultante da ultima operacao merge efetuada pela funcao merge_right*/
static node *
merge_left (node *a)
{
  node *b;
 /* Suponha que antes de executar essa a funcao tenhamos a seguinte configuracao resultante apos merge_right
 *
 *          A  -----------------------> D  --------------> E
 *          |^ <----------------------- |^ <-------------- |^
 *          ||                          ||                 ||
 *          ^|                          ^|                 ^|
 *          B  ---> Ta                  C ---> Td          Te
 *          |^ <---                     |^ <--- 
 *          ||                          ||
 *          ^|                          ^|
 *          Tb                          Tc
 *
 *E que chave de A < chave de E < chave de D  
 *
 */

  for (b = a->prev; b != NULL; b = a->prev)
  { 
   a = merge (b, a);
  }  
/*
 1 iteracao: b == &D || a == &E
 2 iteracao: b == &A || a == &E
*/

 /* Apos executar essa a funcao teremos:
 *                                     
 *                                      A
 *                                      |^
 *                                      ||
 *                                      ^|
 *                                      E  ---------> B ---> Ta 
 *                                      |^ <--------- |^  <---
 *                                      ||            ||
 *                                      ^|            ^|
 *                                      D  ---> Te    Tb 
 *                                      |^ <---
 *                                      ||                 
 *                                      ^|                 
 *                                      C ---> Td          
 *                                      |^ <--- 
 *                                      ||
 *                                      ^|
 *                                      Tc
 *
 */

  return a;
}

/* Essa funcao implementa o merge dos subheaps (isto eh, um merge entre os heaps "filhos") do no a. Esse processo eh efetuado em duas etapas. Na primeira etapa o merge_right eh efetuado. Na segunda etapa o merge_left. No final um ponteiro para a nova raiz eh retornada. Essa funcao eh chamada sempre que retirarmos o elemento cuja chave eh minima.*/
static node *
mergeSubheaps (node *a)
{
  a->child->prev = NULL;//o no filho nao possui mais ponteiro para o no raiz
  node *e;
  e = merge_right (a->child);
  e = merge_left (e);
  a->child = NULL;//o antigo no raiz eh completamente desvinculado ao novo heap
  return e;
}

/* Essa funcao desvincula o subheap do restante do heap. Sempre que formos alterar a chave de um elemento, removemos o subheap encabecado por ele e em seguida efetuamos um merge com a raiz atual a fim de obtermos a nova raiz*/
void
detach_subheap (node *elem)
{
  if (elem->prev->child == elem)
   elem->prev->child = elem->next;/*Caso o subheap a ser removido for o que comeca a lista encadeada de filhos, entao o novo filho que comecara a lista encadeada eh justamente o que vem depois. Assim, o no pai precisa apontar para o seguinte da lista*/
  else
    elem->prev->next = elem->next;/*Se o no nao comeca a lista entao existe um no anterior, a menos que ele seja a raiz do heap. Esse linha faz com que a lista encadeada se mantenha mesmo apos a retirada de um no central*/

  if (elem->next != NULL)
    elem->next->prev = elem->prev;/*Se o no que sucede o elemento a ser retirado existir, entao o no anterior pra o qual o proximo aponta precisa ser o no anterior ao elemento a ser retirado*/

  elem->prev = NULL;
  elem->next = NULL;
}

/* Remove o elemento raiz do heap. Essa funcao retorna portanto um ponteiro que aponta para o elemento raiz. Como esse heap esta organizado na forma de arvore, a remocao do no raiz faz com que os filhos do no retirado ocupem o topo da arvore. Assim, de forma a termos um unico elemento na raiz, eh necessario efetuar um merge entre as subarvores do no retirado*/
node *
heap_pop (Heap *heap)
{
  if(heap->size > 0)//checa se o heap nao esta vazio
  {
   heap->size -= 1;//decrementa o tamanho do heap
   node *r = heap->root;
   if (r->child == NULL)//checa se o no raiz possui filhos
   {
    heap->root = NULL;//se nao possuir entao a raiz do heap passa a ser NULL, isto eh, o heap esta vazio
    return r;
   }
   //caso contrario, o no raiz possui filhos e portanto eh necessario efetuar um merge entre as subarvores do no raiz a fim de obter a nova raiz
   heap->root = mergeSubheaps (heap->root);
   return r;
  }
  return NULL;
}

/* Essa funcao atualiza a chave de um no. Para tal ela acessa o no cujo vertexId eh "i" a partir o endereco armazenado em heap->array[i]. Quando a chave eh atualizada, nada garante que a ordem do heap se manteve. Logo eh necessario desvincular o subheap encabecado pelo elemento atualizado e depois efetuar um merge entre o heap encabecado pela raiz e o subheap retirado*/
void
heap_decrease (Heap *heap, int destinationVertexId, float newValue)
{
  //printf("\nPrevious cost:%lf",heap->array[destinationVertexId]->currentCostToInclude);
  heap->array[destinationVertexId]->currentCostToInclude = newValue;//atualiza o custo 
  //printf("\nNew cost:%lf\n",heap->array[destinationVertexId]->currentCostToInclude);
  node *elem = (heap->array[destinationVertexId]);
  if (elem->prev != NULL)//Checa se o elemento eh a raiz, unico elemento que nao possui anterior
  {
   detach_subheap (elem);//o subheap encabecado pelo elemento cuja chave foi alterada e primeiramente retirado do heap
   heap->root = merge (heap->root, elem);//em seguida reorganiza o heap pra achar a nova posicao do subheap e consequentemente a nova raiz
  }
}

void
heap_init (Heap *heap,int V)
{
  heap->size = 0;
  heap->array = (node**)malloc(V * sizeof(node*));/*Esse vetor guarda a posicao de cada vertice no heap.Isso eh importante para quando modificarmos um elemento*/
  heap->root = NULL;//inicializa o ponteiro raiz com NULL
  for(int i = 0;i<V;i++)
  {
   heap->array[i] = NULL;/*Array[i] contem o endereco de memoria de todos os nos que ja foram inseridos no heap. Ao inicializarmos o heap nenhum elemento foi adcionado. Logo todos os ponteiros sao NULL*/    
  }
}


