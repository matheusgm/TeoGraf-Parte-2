typedef struct fila Fila;

Fila* fila_cria();

void fila_insere(Fila *f, int v);

int fila_vazia(Fila *f);

int fila_retira(Fila *f);

void fila_libera(Fila *f);

