#include<stdlib.h>

typedef unsigned int uint;

// Para facilitar a implementaação, uma entrada da tabela de página contém os dados necessários
// para executar todos os algoritmos de substituição requeridos

typedef struct {
    // Estatísticas da página
    uint totalRead, totalWrite;

    // Bit de controle - Second chance
    uint bit2a;

    // Controle do tempo de acesso - LRU
    uint lastAccessTime;

    int hasPage;
    uint pageID;

}PageEntry;


// A tabela de páginas, implementada como um vetor de páginas devido à simplicidade de implementação
typedef struct {
    //Estatisticas
    uint TotalPageFaults;
    uint TotalFrameCount;

    PageEntry *frameList;
    uint readCount, writeCount;

}PageTable;

// Inicializaçõa da lista
PageTable* pageTableInit(uint frameTotal, uint Pagetotal){
    uint i;

    PageTable* tmp = (PageTable*)malloc(sizeof(PageTable));
    tmp->TotalFrameCount = frameTotal;

    tmp->frameList = (PageEntry*)malloc(frameTotal*sizeof(PageEntry));
    
    for(i=0;i<frameTotal;i++){
        tmp->frameList[i].totalRead = 0;
        tmp->frameList[i].totalWrite = 0;
        tmp->frameList[i].hasPage = 0;
    }

}

void requestPage(PageTable *pt, uint PageID, char mode){
    uint i;
    //Procura pela página na tabela
    for(i = 0; i < pt->TotalFrameCount; i++){
        if(pt->frameList[i].pageID == PageID){ //Página está na tabela
            //Atualiza estatísticas
            switch(mode){
                case 'W':
                    pt->writeCount++; break;
                case 'R':
                    pt->readCount++; break;
                default: //Se um modo de escrita e leitura não for especificado, considere como leitura
                    pt->readCount++; break;
            }
        }
        else{
            //Procura um um lugar vazio para inserir a página, no estilo round-robin
            if(!pt->frameList[i].hasPage){
                pt->frameList[i].hasPage = 1;
                pt->frameList[i].pageID = PageID;
            }
            else{ //Substituir uma página baseado no algoritmo escolhido pelo usuário

            }
        }
    }
}


