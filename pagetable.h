#include<stdlib.h>
#include<time.h>

typedef unsigned int uint;

// Para facilitar a implementaação, uma entrada da tabela de página contém os dados necessários
// para executar todos os algoritmos de substituição requeridos

// Representação de uma página de memória.
// Para facilitar a implementaação, uma entrada da tabela de página contém os dados necessários para executar todos os algoritmos de substituição requeridos.
typedef struct{

    // Bit de controle - Second chance
    uint bit2a;

    // Controle do tempo de acesso - LRU
    time_t lastAccessTime;

    uint pageID;
    struct PageEntry *next;

}PageEntry;

// A tabela de páginas. Esta tabela foi implementada usando uma fila circular, em função da sua simplicidade de implementação e funcionamento com vários algoritmos de substituição.
typedef struct {

    char substitutionAlgorithm; // Inicial do algoritmo de substituição escolhido pelo usuário

    uint TotalFrameCount; /* O total de páginas(frames) que a tabela possui. É definido na inicialização da tabela. */
    uint currentFrameCount; /* Quantas páginas existem na tabela atualmente */

    // Para uso no algoritmo first-come-first-serve
    PageEntry *head;
    uint front, back;

    //Estatisticas
    uint TotalPageFaults;
    uint readCount, writeCount;

}PageTable;

// Inicializa uma tabela de páginas nova.
PageTable* pageTableInit(char substitutionAlgortithm, uint frameTotal, uint Pagetotal);

// Insere um item no fim da fila.
void push(PageTable* pt, uint PageID);

// Remove o primeiro item da fila.
void pop(PageTable* pt, uint PageID);

// Retorna se a tabela de páginas estpa vazia ou não.
int isEmpty( PageTable* pt );

// Retorna se a tabela de páginas está cheia ou não.
int isFull( PageTable* pt );

// Algoritmo de substituição de páginas Second chance.
void replace2a(PageTable* pt, uint newPageID);

// Algoritmo de substuição aleatória.
void replaceRandom(PageTable* pt, uint newPageID);

// Simula uma requisição de página de memória.
void requestPage(PageTable* pt, uint PageID, char mode);

// Limpeza
void delete(PageTable* pt);
