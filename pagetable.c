#include <stdlib.h>
#include <time.h>
//#include <"pagetable.h">

typedef unsigned int uint;

// Representação de uma página de memória.
// Para facilitar a implementaação, uma entrada da tabela de página contém os dados necessários para executar todos os algoritmos de substituição requeridos.
typedef struct {

    // Bit de controle - Second chance
    uint bit2a;

    // Controle do tempo de acesso - LRU
    time_t lastAccessTime;

    uint pageID;
    PageEntry *next;

} PageEntry;


// A tabela de páginas. Esta tabela foi implementada usando uma fila circular, em função da sua simplicidade de implementação e funcionamento com vários algoritmos de substituição.
typedef struct {

    char substitutionAlgorithm; // Inicial do algoritmo de substituição escolhido pelo usuário

    uint TotalFrameCount; /* O total de páginas(frames) que a tabela possui. É definido na inicialização da tabela. */
    uint currentFrameCount; /* Quantas páginas existem na tabela atualmente */

    // Último elemento inserido na lista.
    PageEntry *head;

    //Estatisticas
    uint TotalPageFaults;
    uint readCount, writeCount;

}PageTable;

// Inicializa uma tabela de páginas nova. A tabela é uma fila circular, onde cada elemento representa uma página de memória.
PageTable* pageTableInit(uint frameTotal, uint Pagetotal) {
    PageTable *tmp = (PageTable*)malloc(sizeof(PageTable));
    tmp->TotalFrameCount = frameTotal;
    tmp->currentFrameCount = 0;
    tmp->TotalPageFaults = 0;
    tmp->readCount = 0;
    tmp->writeCount = 0;
    tmp->head = NULL;

    return tmp;
}

// Retorna se a tabela de páginas estpa vazia ou não.
int isEmpty( PageTable* pt ) {
    return pt->currentFrameCount;
}

// Retorna se a tabela de páginas está cheia ou não.
int isFull ( PageTable* pt ) {
    return pt->currentFrameCount == pt->TotalFrameCount;
}

// Insere um item na tabela.
void push(PageTable* pt, uint PageID) {
    PageEntry *tmp = (PageEntry*)malloc(sizeof(PageEntry));
    tmp->pageID = PageID;
    tmp->bit2a = 1;
    time(tmp->lastAccessTime);

    pt->currentFrameCount++;

    if(isEmpty(pt)){
        tmp->next = &tmp;
    }else{
        tmp->next = pt->head;
    }
    pt->head = tmp;
}

// Remove um item específico da tabela.
void pop(PageTable* pt, uint PageID){
    PageEntry *tmp = pt->head, *prev;

    //Nada acontece se a tabela estiver vazia
    if (!pt->head) return;

    while(1){
        if(tmp->pageID == PageID){
            // A lista contém apenas 1 elemento
            if(tmp->next == tmp){
                pt->head = NULL;
                free(tmp);
                pt->currentFrameCount--;
                break;
            }

            // O elemento a ser removido é o primeiro
            if(pt->head == tmp){
                while(prev->next != pt->head){
                    prev = prev->next;
                }
                pt->head = pt->head->next;
                prev = prev->next;

                free(tmp);
                pt->currentFrameCount--;
                break;
            }

            // O elemento a ser removido é o último
            if(tmp->next == pt->head){
                prev->next = pt->head;
                free(tmp);
            }

            // Caso geral
            prev->next = tmp->next;
            free(tmp);

        }
        prev ->next = tmp;
        tmp = tmp->next;
    }
}

// Algoritmo de substituição Second chance. A tabela de páginas é percorrida e cada elemento é marcado como já visitado. 
void replace2a(PageTable* pt, uint PageID){
    PageEntry *tmp = pt->head;
    while(tmp->bit2a){
        tmp->bit2a = 0;
        tmp = tmp->next;
    }
    pop(pt, tmp->pageID);
}

// Simula uma requisição de página de memória. retorna 1 se a página foi encontrada e 0 caso contrário
void requestPage(PageTable* pt, uint PageID, char mode){
    PageEntry *tmp = pt->head;
    do{
        if(tmp->pageID == PageID){ // A página está na tabela
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
        tmp = tmp->next;
    }while(tmp != pt->head);

    // A página não está na tabela
    pt->TotalPageFaults++;

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
}