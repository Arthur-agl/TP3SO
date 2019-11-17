#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pagetable.h"

typedef unsigned int uint;

// Inicializa uma tabela de páginas nova. A tabela é uma fila circular, onde cada elemento representa uma página de memória.
// A fila foi implementada usando um vetor. Dessa forma, O algoritmo FCFS pôde ser implementado mais facilmente, enquanto os outros algoritmos se beneficiam do acesso indexado
PageTable* pageTableInit(char substitutionAlgorithm, uint frameTotal, uint Pagetotal) {
    PageTable *tmp = (PageTable*)malloc(sizeof(PageTable));
    tmp->TotalFrameCount = frameTotal;
    tmp->currentFrameCount = 0;
    tmp->TotalPageFaults = 0;
    tmp->readCount = 0;
    tmp->writeCount = 0;
    tmp->head = NULL;

    // Para uso no algoritmo first-come-first-serve
    tmp->front = 0; tmp->back=0;

    // A fila circular precisa de um espaço vazio para diferenciar o estado cheio do estado vazio
    if(substitutionAlgorithm == 'f'){
        tmp->head = (PageEntry*)malloc((frameTotal+1)*sizeof(PageEntry));
    }else{
        tmp->head = (PageEntry*)malloc((frameTotal)*sizeof(PageEntry));
    }

    return tmp;
}

// Retorna se a tabela de páginas está vazia ou não.
int isEmpty( PageTable* pt ) {
    return pt->currentFrameCount;
}

// Retorna se a tabela de páginas está cheia ou não.
int isFull ( PageTable* pt ) {
    return pt->currentFrameCount == pt->TotalFrameCount;
}

// Insere um item na tabela.
void push(PageTable* pt, uint PageID) {
    uint i = pt->front;
    pt->head[i].pageID = PageID;
    pt->head[i].bit2a = 1;
    time(&(pt->head[i].lastAccessTime));

    pt->front +=1;
    pt->front %= pt->TotalFrameCount;
    pt->currentFrameCount++; // atualiza o total de quadros ocupados

}

// Algoritmo de substituição Second chance. A tabela de páginas é percorrida e cada elemento é marcado como já visitado.
void replace2a(PageTable* pt, uint newPageID){
    uint i;
    // O algoritmo fará no máximo 2 passadas pela tabela de páginas
    rep:
        for(i=0; i<pt->TotalFrameCount;i++){
            if(!pt->head[i].bit2a){
                // Outros valores não são utilizados pelo second chance
                pt->head[i].pageID = newPageID;
                pt->head[i].bit2a = 1;
                return;
            }else{
                pt->head[i].bit2a = 0;
            }
        }
    goto rep;
}

void replaceRandom(PageTable* pt, uint newPageID){
    uint i;
    time_t t;
    srand(time(&t));
    i = rand() % pt->TotalFrameCount;
    pt->head[i].pageID = newPageID;
}

/*void replace(PageTable* pt, uint PageID, Pagina vitima)
{
    vitima->bit = invalido
    pt[PageID] = vitima-frame
    vitima->frame = 0
}

/*void fifoReplace(PageTable* pt, uint PageID)
{
    vitima = selecionaPrimeiraPagina()
    replpace(pt, PageID, vitima)
}*/

/*void lruReplace(PageTable* pt, unit PageID)
{
    achou = False
    vitima = PageTable[0]
    for(pagina: PageTable)
    {
        if(pagina->usado == 0 && !achou)
        {
            vitima = pagina
            achou = True
        }
        pagina->usado = 0
    }
    replace(pt, PageID, vitima) 
}
*/
// Simula uma requisição de página de memória. retorna 1 se a página foi encontrada e 0 caso contrário
void requestPage(PageTable* pt, uint PageID, char mode){
    uint i;
    for(i=0; i < pt->currentFrameCount; i++){
        if(pt->head[i].pageID == PageID){ // A página está na tabela
            switch(mode){
                case 'W':
                    pt->writeCount++; break; //Simula a escrita de dados na página
                case 'R':
                    pt->readCount++; break; //Simula a leitura de um dado da página
                default: //se nenhum modo de acesso à página for definido, escolher o modo leitura como padrão
                    pt->readCount++;break;
            }
            return;
        }
    }

    // A página não está na tabela
        pt->TotalPageFaults++; // Simula um pageFault

        if(!isFull(pt)){ // Insere a nova página num espaço vazio da tabela
            push(pt, PageID);
        }else{ // Chama um algoritmo de substituição
            switch(pt->substitutionAlgorithm){
                case '2':
                    replace2a(pt, PageID); break;
                case 'r':
                    replaceRandom(pt, PageID); break;
                default:
                printf("Erro!");
                exit(1);
            }
        }
        return;
}

void delete(PageTable* pt){
    free(pt->head);
    free(pt);
}
