#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pagetable.h"
#include "listaTAD.h"

typedef unsigned int uint;

// Inicializa uma tabela de páginas nova. Cada entrada na tabela representa uma página.
//Cada entrada é uma PageEntry, onde basicamente tem o frame a que essa pagina corresponde, um bit de validação e informações adicionais
PageTable *pageTableInit(char substitutionAlgorithm, uint PageTotal)
{
    PageTable *tmp = (PageTable *)malloc(sizeof(PageTable));
    tmp->substitutionAlgorithm = substitutionAlgorithm;
    tmp->readCount = 0;
    tmp->writeCount = 0;
    tmp->TotalPages = PageTotal;
    tmp->pageList = (PageEntry *)malloc((PageTotal) * sizeof(PageEntry));

    for (int i = 0; i < PageTotal; i++)
    {
        tmp->pageList[i].valid = 0;
        tmp->pageList[i].frameID = -1;
    }

    //Se o algoritmo de reposiçao é o FIFO, incializa a lista encadeada
    if (substitutionAlgorithm == 'f')
    {
        FLVazia(&tmp->ListaFIFO);
    }

    return tmp;
}

// Inicializa uma tabela de quadros. Cada entrada da tabela é um quadro localizado na memória física
FrameTable *frameTableInit(uint totalFrames)
{
    FrameTable *tmp = (FrameTable *)malloc(sizeof(FrameTable));
    tmp->totalFrames = totalFrames;
    tmp->frameList = (Frame *)malloc(totalFrames * sizeof(Frame));

    for (int i = 0; i < totalFrames; i++)
    {
        tmp->frameList[i].ocupado = 0;
    }

    return tmp;
}

//Atualiza estatisticas
void updateStats(PageTable *pt, char mode)
{
    switch (mode)
    {
    case 'W':
        pt->writeCount++;
        break; //Simula a escrita de dados na página
    case 'R':
        pt->readCount++;
        break; //Simula a leitura de um dado da página
    default:   //se nenhum modo de acesso à página for definido, escolher o modo leitura como padrão
        pt->readCount++;
        break;
    }
}

void replace(PageTable *pt, uint PageID, PageEntry vitima)
{
    vitima.valid = 0;
    pt->pageList[PageID].frameID = vitima.frameID;
    vitima.frameID = -1;
    pt->pageList[PageID].valid = 1;
    pt->pageList[PageID].accessed = 1;
    pt->pageList[PageID].bit2a = 1;
}

void replaceFIFO(PageTable *pt, uint PageID)
{
    uint vitimaID = Dequeue(&pt->ListaFIFO);
    PageEntry vitima = pt->pageList[vitimaID];
    replace(pt, PageID, vitima);
}

void replaceRandom(PageTable *pt, uint PageID)
{
    uint vitimaID = rand() % pt->TotalPages;
    PageEntry vitima = pt->pageList[vitimaID];
    replace(pt, PageID, vitima);
}

// Carrega uma pagina no quadro indicado e atualiza tabela de paginas.
void carregaPagina(PageTable *pt, FrameTable *ft, uint PageID, char mode)
{
    int frameID = -1;
    for (int i = 0; i < ft->totalFrames; i++)
    {
        //Retorna o primeiro quadro livre
        if (!ft->frameList[i].ocupado)
        {
            frameID = i;
            break;
        }
    }

    //Memoria cheia
    if (frameID < 0)
    {
        //Inicia algoritmo de reposição
        switch (pt->substitutionAlgorithm)
        {
        case 'r':
            replaceRandom(pt, PageID);
            break;

        case 'f':
            replaceFIFO(pt, PageID);
            break;

        default:
            printf("Erro: Algoritmo de reposição não reconhecido\n");
            break;
        }
        return;
    }

    pt->pageList[PageID].frameID = frameID;
    pt->pageList[PageID].valid = 1;
    pt->pageList[PageID].accessed = 1;
    pt->pageList[PageID].bit2a = 1;

    // Se o algoritmo é FIFO, adiciona o id da nova pagina à lista encadeada
    if (pt->substitutionAlgorithm == 'f')
    {
        TipoItem x;
        x.pageID = PageID;
        Insere(x, &pt->ListaFIFO);
    }
}

// Algoritmo de substituição Second chance. A tabela de páginas é percorrida e cada elemento é marcado como já visitado.
/*void replace2a(PageTable* pt, uint newPageID){
    uint i;
    // O algoritmo fará no máximo 2 passadas pela tabela de páginas
    rep:
        for(i=0; i<pt->TotalPages;i++){
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
void acessaPagina(PageTable *pt, FrameTable *ft, uint PageID, char mode)
{
    //Pagina está na memoria
    if (pt->pageList[PageID].valid)
    {
        updateStats(pt, mode);
        return;
    }

    // A página não está na memória
    pt->TotalPageFaults++; // Simula um pageFault

    //Carrega pagina em um quadro
    carregaPagina(pt, ft, PageID, mode);
    updateStats(pt, mode);
    return;
}

void delete (PageTable *pt)
{
    free(pt->pageList);
    free(pt);
}
