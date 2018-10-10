typedef struct pilha Pilha;

extern Pilha* pilha_cria();

extern void pilha_push(Pilha* p, int v);

extern int pilha_vazia(Pilha* p);

extern int pilha_pop(Pilha* p);

extern void pilha_libera(Pilha* p);

