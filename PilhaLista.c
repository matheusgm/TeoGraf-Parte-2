#include <stdlib.h>
#include <stdio.h>
/* nó da lista para armazenar valores reais */
typedef struct no No;
struct no {
    int info;
    No *prox;
};

typedef struct pilha Pilha;
struct pilha{
    No* prim;
};

Pilha* pilha_cria(){
    Pilha* p = (Pilha*) malloc(sizeof(Pilha));
    if (p==NULL) {
       exit(1);
    }
    p->prim = NULL;
    return p;
}

void pilha_push(Pilha* p, int v){
    No* n = (No*) malloc(sizeof(No));
    if (n==NULL) {
        exit(1);
    }
    n->info = v;
    n->prox = p->prim;
    p->prim = n;
}

int pilha_vazia(Pilha* p){
    if(p->prim == NULL){
        return 1;
    }
    return 0;
}

int pilha_pop(Pilha* p){
    No* t;
    int v;
    if (pilha_vazia(p)) {
        exit(1); /* aborta programa */
    }
    t = p->prim;
    v = t->info;
    p->prim = t->prox;
    free(t);
    return v;
}

void pilha_libera(Pilha* p){
    No *t, *q = p->prim;
    while (q!=NULL)
    {
        t = q->prox;
        free(q);
        q = t;
    }
    free(p);
}



