#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"pagetable.h"

#define VIRTUAL_ADDRESS_SIZE 32
typedef unsigned int uint;

unsigned int bitsToDiscard(uint pageSize){
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
    uint pageTotal; //Total de páginas acessíveis por um processo
    uint frameTotal; //Total de quadros

    uint totalOperations = 0; // Total de operações de leitura/escrita (linhas do arquivo de entrada)
    uint addr; // Endereço que será lido no arquivo
    char rw; // Modo de acesso à memória que será lido no arquivo
    uint pageID; // Página de memória, deduzida

    // --- Tratamento dos argumentos
    if(argc < 5){
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

    pageSize = atoi(argv[3]); // Tamanho da página, em KB
    totalPhysicalMem = atoi(argv[4]); // Tamanho da memória física, em KB

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
    
    // Bits que serão descartados na hora de calcular o ID da página
    s = bitsToDiscard(pageSize*1024); //o cálculo é feito com o o valor em bytes

    // Total de páginas possível
    pageTotal = 2 << (VIRTUAL_ADDRESS_SIZE - s);

    // Total de quadros na tabela de páginas. t = KB disponíveis na memória física / tamanho da página, em KB
    frameTotal = totalPhysicalMem/pageSize;

    // --- Montagem da tabela de páginas
    PageTable *pt = pageTableInit(substitutionAlgortithm[0], pageTotal);

    // --- Montagem da tabela de quadros
    FrameTable *ft = frameTableInit(frameTotal);

    //Leitura do arquivo e gravação das estatísticas
    printf("Arquivo de entrada: %s\n", inputFile);
    printf("Tamanho da memoria: %d KB\n", totalPhysicalMem);
    printf("Tamanho de pagina: %d KB\n", pageSize);
    printf("Tamanho da tabela de frames: %u quadros\n", frameTotal);
    printf("Tecnica de reposicao escolhida: ");
    switch((char)substitutionAlgortithm[0]){
        case 'l':
            printf("LRU - least recently used\n");
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
        totalOperations++;
        pageID = addr >> s;
        acessaPagina(pt, ft, pageID, rw);
    }

    // Imprimir relatório com estatísticas
    printf("Estatisticas do simulador: \n");
    printf("Total de acessos: %u\n", totalOperations);
    printf("Paginas lidas (page faults): %u\n", pt->pagesRead);
    printf("Paginas escritas: %u\n", pt->pagesWritten);

    // Limpeza
    delete(pt);
    fclose(f);
    return 0;
}
