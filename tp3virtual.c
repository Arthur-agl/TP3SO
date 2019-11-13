#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define VIRTUAL_ADDRESS_SIZE 32
typedef unsigned int uint;

unsigned int bitsToDiscard(unsigned int pageSize){
    uint tmp = pageSize;
    uint s = 0;
    
    while (tmp > 1) {
        tmp = tmp >> 1;
        s++;
    }

    return s;
}

int main(int argc, char** argv){
    // Variáveis de entrada da linha de comando
    char* substitutionAlgortithm[10];
    char* inputFile[50];
    uint pageSize;
    uint totalPhysicalMem;

    uint s; //bits a serem descartados para determinar o número da página
    uint pageTotal;
    uint frameTotal;

    uint addr;
    char rw;
    uint requestedPage;

    // Tratamento dos argumentos
    if(argc < 4){
        printf("Numero incorreto de argumentos!\n"); 
        return -1;
    }

    strcpy(substitutionAlgortithm, argv[1]);

    if ( strcmp(substitutionAlgortithm,"lru") != 0 
    && strcmp(substitutionAlgortithm,"2a")  != 0
    && strcmp(substitutionAlgortithm,"fifo") != 0
    && strcmp(substitutionAlgortithm,"random") != 0 )
    {
        printf("Algoritmo de substituicao invalido!\n");
        return -1;
    }

    pageSize = argv[3];
    totalPhysicalMem = argv[4];

    if(pageSize < 2 && pageSize > 64){
        printf("O tamanho da pagina deve ser entre 2 e 64 KB, inclusivo.\n");
        return -1;
    }
    if(totalPhysicalMem < 16 && totalPhysicalMem > 16384){
        printf("O total de memoria fisica deve ser entre 16 e 16384 KB, inclusivo.\n");
        return -1;
    }

    strcpy(inputFile, argv[2]);
    FILE *f = fopen(inputFile,"r");
    if (f == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return -1;
    }

    // Bits que serão descartados na hora de calcular a página
    s = bitsToDiscard(pageSize);

    // Total de páginas possível
    pageTotal = totalPhysicalMem / pageSize;

    // Total de frames na tabela de página
    frameTotal = VIRTUAL_ADDRESS_SIZE - s;

    // Montagem da tabela de páginas
    //PageTable *p = pageTableInit(pageTotal, frameTotal);

    //Leitura do arquivo e gravação das estatísticas
    printf("Arquivo de entrada: %s\n", inputFile);
    printf("Tamanho da memoria: %u KB\n", totalPhysicalMem);
    printf("Tamanho de pagina: %u KB\n", pageSize);
    printf("Tecnica de reposicao escolhida: ");
    switch((char)substitutionAlgortithm[0]){
        case 'l':
            printf("LRU - menos usado recentemente\n");
            break;

        case '2':
            printf("SECOND CHANCE - segunda chance\n");
            break;

        case 'f':
            printf("FIFO - first in, first out\n");
            break;

        case 'r':
            printf("RANDOM\n");
            break;

        default:
            break;
    }

    printf("Executando o simulador de acesso a memoria ... \n");

    while(fscanf (f,"%x %c", &addr, &rw) != EOF){
        requestedPage = addr >> s;
        //p->requestPage(requestedPage);
    }

    // Imprimir relatório com estatísticas
    printf("Estatísticas do simulador: \n");
    printf("Paginas lidas:\n");
    printf("Paginas escritas:\n");
    printf("Total de page faults:\n");
    printf("Tempo total de simulacao:\n");


    // Limpeza
    fclose(f);
    return 0;
}