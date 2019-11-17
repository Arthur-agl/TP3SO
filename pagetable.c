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
        tmp->pageList[i].accessed = 0;
        tmp->pageList[i].frameID = -1;
    }

    //Se o algoritmo de reposiçao é o FIFO, incializa a fila
    if (substitutionAlgorithm == 'f')
    {
        FLVazia(&tmp->Fila);
    }

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

//Retorna o quadro da pagina vitima e atualiza tabela de paginas
uint replace(PageEntry *vitima)
{
    vitima->valid = 0;
    uint victim_frame = vitima->frameID;
    vitima->frameID = -1;
    return victim_frame;
}

uint replaceFIFO(PageTable *pt)
{
    //Primeiro elemento da lista
    uint vitimaID = Dequeue(&pt->Fila);
    PageEntry *vitima = &pt->pageList[vitimaID];
    return replace(vitima);
}

uint replaceRandom(PageTable *pt, FrameTable *ft)
{
    //Escolhe um quadro aleatorio para ser substituído
    uint vitimaFrame = rand() % ft->totalFrames;

    //Obtem a pagina a qual aquele quadro se refere
    uint vitimaPage = ft->frameList[vitimaFrame].pageID;
    PageEntry *vitima = &pt->pageList[vitimaPage];

    return replace(vitima);
}

uint replaceLru(PageTable *pt, FrameTable *ft)
{
    int achou = 0;
    PageEntry *vitima = &pt->pageList[0];
    Frame current_frame;
    int current_page;

    //Procura entre as paginas alocadas em quadros
    for(int i = 0; i < ft->totalFrames; i++)
    {
        //Quadro atual
        current_frame = ft->frameList[i];

        //Pagina atual a qual o quadro atual se refere
        current_page = current_frame.pageID;

        //Verifica se a pagina atual foi acessada recentemente
        if(pt->pageList[current_page].accessed == 0 && !achou)
        {
            //Se não foi, ela é a vítima
            vitima = &pt->pageList[current_page];
            achou = 1;
        }

        //Continua resetando os bits de acesso recente
        pt->pageList[current_page].accessed = 0;
    }

    return replace(vitima);
}


// Carrega uma pagina no quadro indicado e atualiza tabela de paginas.
void carregaPagina(PageTable *pt, FrameTable *ft, uint PageID)
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
            frameID = replaceRandom(pt, ft);
            break;

        case 'f':
            frameID = replaceFIFO(pt);
            break;

        case 'l':
            frameID = replaceLru(pt, ft);
            break;

        default:
            printf("Erro: Algoritmo de reposição não reconhecido\n");
            break;
        }
        return;
    }

    //Pagina é alocada no quadro frameID
    pt->pageList[PageID].frameID = frameID;
    //frameID guarda a pagina que está alocada nele
    ft->frameList[frameID].pageID = PageID;
    pt->pageList[PageID].valid = 1;
    pt->pageList[PageID].accessed = 1;
    pt->pageList[PageID].bit2a = 1;

    // Se o algoritmo é FIFO, adiciona o id da nova pagina à fila
    if (pt->substitutionAlgorithm == 'f')
    {
        TipoItem x;
        x.pageID = PageID;
        Insere(x, &pt->Fila);
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

// Simula uma requisição de página de memória.
void acessaPagina(PageTable *pt, FrameTable *ft, uint PageID, char mode)
{
    //Pagina está na memoria
    if (pt->pageList[PageID].valid)
    {
        updateStats(pt, mode);
        pt->pageList[PageID].accessed = 1;
        return;
    }

    // A página não está na memória
    pt->TotalPageFaults++; // Simula um pageFault

    //Carrega pagina em um quadro
    carregaPagina(pt, ft, PageID);
    updateStats(pt, mode);
    return;
}

void delete (PageTable *pt)
{
    free(pt->pageList);
    free(pt);
}
