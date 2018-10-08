typedef struct elemento Elemento;

/* Função de criação: retorna uma lista vazia */
Elemento* lst_cria();

/* Inserção no inicio: retorna a lista atualizada */
Elemento* lst_insere(Elemento *lst, int val,float peso);

/* função imprime: imprime valores dos elementos */
void lst_imprime(Elemento *lst);

/* função vazia: retorna 1 se vazia ou 0 se não vazia */
int lst_vazia(Elemento *lst);

/* função busca: busca um elemento na lista */
Elemento* busca(Elemento *lst, int v);

void lst_libera(Elemento *lst);

Elemento* proximaLista(Elemento *lst);

int getItem(Elemento *lst);

float getWeight(Elemento *lst);

int getTamanhoLista(Elemento *lst);

