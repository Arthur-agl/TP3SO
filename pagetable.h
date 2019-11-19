#include <stdlib.h>
#include <time.h>

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
    time_t lastAccessed;

    // Bit para validade
    uint valid;

    // Indica de a página está 'suja' e precisa ser escrita no disco
    int dirty;

    // ID do quadro em que a página está alocada
    int frameID;

} PageEntry;

// A tabela de páginas.
typedef struct
{

    char substitutionAlgorithm; /* Inicial do algoritmo de substituição escolhido pelo usuário. */

    uint TotalPages; /* O total de páginas que a tabela possui. É definido na inicialização da tabela. */

    // Lista de Paginas
    PageEntry *pageList;

    // Marcador do primeiro item na fila. Para uso no algoritmo FIFO
    uint firstInserted;

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

void deleteFt (FrameTable *ft);
