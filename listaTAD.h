#include <stdlib.h>

//Estruturas para a Lista Encadeada
typedef int TipoChave;

typedef struct {
    int pageID;
} TipoItem;

typedef struct TipoCelula *TipoApontador;

typedef struct TipoCelula {
    TipoItem Item;
    TipoApontador Prox;
} TipoCelula;

typedef struct {
    TipoApontador Primeiro, Ultimo;
} TipoLista;

void FLVazia(TipoLista *Lista);

void Insere(TipoItem x, TipoLista *Lista);

uint Dequeue(TipoLista *Lista);
