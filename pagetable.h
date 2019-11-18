#include <stdlib.h>
#include <time.h>
#include "listaTAD.h"

typedef unsigned int uint;

// Para facilitar a implementaação, uma entrada da tabela de página contém os dados necessários
// para executar todos os algoritmos de substituição requeridos

// Representação de uma página de memória.
// Para facilitar a implementaação, uma entrada da tabela de página contém os dados necessários para executar todos os algoritmos de substituição requeridos.
typedef struct
{

    // Bit de controle - Second chance
    uint bit2a;

    // Controle do tempo de acesso - LRU
    uint accessed;

    // Bit para validade
    uint valid;

    // Modo em que a página foi utilizada mais recentemente
    char recent_mode;

    // ID do frame em que a página está alocada
    int frameID;

} PageEntry;

// A tabela de páginas. Esta tabela foi implementada usando uma fila circular, em função da sua simplicidade de implementação e funcionamento com vários algoritmos de substituição.
typedef struct
{

    char substitutionAlgorithm; // Inicial do algoritmo de substituição escolhido pelo usuário

    uint TotalPages; /* O total de páginas que a tabela possui. É definido na inicialização da tabela. */

    // Lista de Paginas
    PageEntry *pageList;

    // Fila auxiliar para o uso do FIFO
    TipoLista Fila;

    //Estatisticas (páginas lidas do disco e páginas escritas no disco)
    uint pagesRead, pagesWritten;

} PageTable;

typedef struct
{
    uint ocupado;
    int pageID;

} Frame;

typedef struct
{

    Frame *frameList;
    uint totalFrames;

} FrameTable;

// Inicializa uma tabela de páginas nova.
PageTable *pageTableInit(char substitutionAlgortithm, uint PageTotal);

//Inicializa a memoria
FrameTable *frameTableInit(uint totalFrames);

// Simula uma requisição de página de memória.
void acessaPagina(PageTable *pt, FrameTable *ft, uint PageID, char mode);

// Limpeza
void delete (PageTable *pt);
