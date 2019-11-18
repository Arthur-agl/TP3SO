#include<stdlib.h>
#include<time.h>

typedef unsigned int uint;

// Para facilitar a implementaação, uma entrada da tabela de página contém os dados necessários
// para executar todos os algoritmos de substituição requeridos

// Representação de um quadro na memória.
// Para facilitar a implementaação, uma entrada da tabela de página contém os dados necessários para executar todos os algoritmos de substituição requeridos.
typedef struct{
    // Identificador
    uint pageID;

    // Bit de controle - Second chance
    uint bit2a;

    // Controle do tempo de acesso - LRU
    time_t lastAccessTime;

    // Dirty bit - indica se uma escrita na memória será necessária quando a página sair da tabela de quadros
    int dirty;

    // Para uso no algoritmo FIFO - índice do primeiro elemento que foi inserido na tabela
    uint first;

}PageEntry;

// A tabela de páginas. Esta tabela foi implementada usando um vetor, em função da sua simplicidade de implementação e funcionamento com vários algoritmos de substituição.
typedef struct {

    char substitutionAlgorithm; // Inicial do algoritmo de substituição escolhido pelo usuário

    uint TotalFrameCount; /* O total de páginas(frames) que a tabela possui. É definido na inicialização da tabela. */
    uint currentFrameCount; /* Quantas páginas existem na tabela atualmente */

    // Para uso no algoritmo first-come-first-serve
    PageEntry *head;

    //Estatisticas
    uint TotalPageFaults;
    uint readCount, writeCount;

}PageTable;

// Inicializa uma tabela de páginas nova.
PageTable* pageTableInit(char substitutionAlgortithm, uint frameTotal, uint Pagetotal);

// Simula uma requisição de página de memória.
void requestPage(PageTable* pt, uint PageID, char mode);

// Limpeza
void delete(PageTable* pt);
