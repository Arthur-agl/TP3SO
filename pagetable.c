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
    tmp->pagesRead = 0;
    tmp->pagesWritten = 0;
    tmp->TotalPages = PageTotal;
    tmp->pageList = (PageEntry *)malloc((PageTotal) * sizeof(PageEntry));

    for (int i = 0; i < PageTotal; i++)
    {
        tmp->pageList[i].valid = 0;
        tmp->pageList[i].lastAccessed = 0;
        tmp->pageList[i].frameID = -1;
    }

    //Se o algoritmo de reposiçao é o FIFO, este contador será utilizado
    tmp->firstInserted = 0;


    return tmp;
}

// Inicializa uma tabela de quadros. Cada entrada da tabela é um quadro localizado na memória física
// Cada entrada da tabela é um Frame, que contem a pagina carregada nele e um bit pra verificar se está ocupado
FrameTable *frameTableInit(uint totalFrames)
{
    FrameTable *tmp = (FrameTable *)malloc(sizeof(FrameTable));
    tmp->totalFrames = totalFrames;
    tmp->frameList = (Frame *)malloc(totalFrames * sizeof(Frame));

    for (int i = 0; i < totalFrames; i++)
    {
        tmp->frameList[i].ocupado = 0;
        tmp->frameList[i].pageID = -1;
    }

    return tmp;
}

//Retorna o quadro da pagina vitima e atualiza tabela de paginas
uint replace(PageTable *pt, PageEntry *vitima)
{   
    // Verifica se a página está suja e precisa de escrita na memória
    if(vitima->dirty){
        pt->pagesWritten++;
    }
    // A página que sair da memória física(frametable) tem seus contadores zerados
    vitima->valid = 0;
    vitima->bit2a = 1;
    vitima->lastAccessed = 0;
    uint victim_frame = vitima->frameID;
    vitima->frameID = -1;
    return victim_frame;
}

uint replace2a(PageTable* pt, FrameTable *ft){
    uint i;
    int found = 0;
    PageEntry *vitima = NULL;
    // O algoritmo fará no máximo 2 passadas pela tabela de páginas
    while(!found){
        for(i=0; i<ft->totalFrames;i++){
            if(pt->pageList[ft->frameList[i].pageID].bit2a == 1){
                pt->pageList[ft->frameList[i].pageID].bit2a = 0;
            }else{
                vitima = &pt->pageList[ft->frameList[i].pageID];
                found = 1;
                break;
            }
        }
    }

    return replace(pt, vitima);
}

uint replaceFIFO(PageTable *pt, FrameTable *ft)
{
    PageEntry *vitima;

    //Primeiro elemento da lista
    uint vitimaFrameID = pt->firstInserted;
    pt->firstInserted +=1;
    pt->firstInserted %= ft->totalFrames;

    vitima = &pt->pageList[ft->frameList[vitimaFrameID].pageID];

    return replace(pt,vitima);
}

uint replaceRandom(PageTable *pt, FrameTable *ft)
{
    time_t t;
    //Escolhe um quadro aleatorio para ser substituído
    srand(time(&t));
    uint vitimaFrame = rand() % ft->totalFrames;

    //Obtem a pagina a qual aquele quadro se refere
    uint vitimaPage = ft->frameList[vitimaFrame].pageID;
    PageEntry *vitima = &pt->pageList[vitimaPage];

    return replace(pt, vitima);
}

uint replaceLru(PageTable *pt, FrameTable *ft)
{
    uint menor = 0;

    PageEntry *vitima;

    Frame current_frame;
    uint current_page;

    //Procura entre as paginas alocadas em quadros
    for(int i = 0; i < ft->totalFrames; i++)
    {
        //Quadro atual
        current_frame = ft->frameList[i];

        //Pagina atual a qual o quadro atual se refere
        current_page = current_frame.pageID;

        //Verifica se a página foi menos acessada do que o mínimo atual
        if(pt->pageList[current_page].lastAccessed < pt->pageList[menor].lastAccessed)
        {
            menor = pt->pageList[current_page].frameID;
        }

    }
    vitima = &pt->pageList[ft->frameList[menor].pageID];
    return replace(pt, vitima);
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
        //Inicia algoritmo de reposição. Cada função retorna um frameID escolhido de acordo com o algoritmo especificado
        switch (pt->substitutionAlgorithm)
        {
        case 'r':
            frameID = replaceRandom(pt, ft);
            break;

        case 'f':
            frameID = replaceFIFO(pt, ft);
            break;

        case 'l':
            frameID = replaceLru(pt, ft);
            break;
        
        case '2':
            frameID = replace2a(pt, ft);
            break;

        default:
            printf("Erro: Algoritmo de reposicao nao reconhecido\n");
            break;
        }
    }

    // A página nova é alocada no quadro frameID
    pt->pageList[PageID].frameID = frameID;
    //frameID guarda a pagina que está alocada nele
    ft->frameList[frameID].pageID = PageID;
    //frameID agora está ocupado
    ft->frameList[frameID].ocupado = 1;

    pt->pageList[PageID].dirty = 0;
    pt->pageList[PageID].valid = 1;
    pt->pageList[PageID].lastAccessed = 1;
    pt->pageList[PageID].bit2a = 1;
}

// Simula uma requisição de página de memória.
void acessaPagina(PageTable *pt, FrameTable *ft, uint PageID, char mode)
{
    //Pagina está na memoria
    if (pt->pageList[PageID].valid)
    {
        time(&pt->pageList[PageID].lastAccessed);
        pt->pageList[PageID].bit2a = 1;
        if(mode == 'W'){
            pt->pageList[PageID].dirty = 1;
        }
        return;
    }

    // A página não está na memória
    pt->pagesRead++; // Simula um pageFault

    //Carrega a pagina faltante em um quadro
    carregaPagina(pt, ft, PageID, mode);
    return;
}

void delete (PageTable *pt)
{
    free(pt->pageList);
    free(pt);
}

void deleteFt (FrameTable *ft){
    free(ft->frameList);
    free(ft);
}
