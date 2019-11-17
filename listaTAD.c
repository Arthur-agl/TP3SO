#include <stdlib.h>
#include "listaTAD.h"

// Algumas funções foram retiradas do livro "Projeto de Algoritmos" do professor Nivio Ziviani

typedef unsigned int uint;

void FLVazia(TipoLista *Lista)
{
    Lista->Primeiro = (TipoApontador)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Primeiro;
    Lista->Primeiro->Prox = NULL;
}

int Vazia(TipoLista Lista)
{
    return (Lista.Primeiro == Lista.Ultimo);
}

void Insere(TipoItem x, TipoLista *Lista)
{
    Lista->Ultimo->Prox = (TipoApontador)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Ultimo->Prox;
    Lista->Ultimo->Item = x;
    Lista->Ultimo->Prox = NULL;
}

uint Dequeue(TipoLista *Lista)
{
    if(Vazia(*Lista))
    {
        printf("Erro: Tentativa de remoção em uma lista vazia\n");
        return;
    }

    //Celular "header"
    TipoApontador celASerRetirada = Lista->Primeiro;

    //Primeiro item da lista
    TipoApontador vitima = Lista->Primeiro->Prox;
    uint vitimaID = vitima->Item.pageID;
   
    //Avança o "header" em uma posiçao
    Lista->Primeiro = Lista->Primeiro->Prox;
    Lista->Primeiro->Item.pageID = -1;

    //Remove o "header antigo"
    free(celASerRetirada);
    
    return vitimaID;
}