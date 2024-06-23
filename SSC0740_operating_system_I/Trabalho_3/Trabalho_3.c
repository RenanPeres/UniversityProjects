//Login: gso14
//Trabalho de Sistemas Operacionais - Gerenciamento de memória
//Programa com simulador  de gerenciamento de memória virtual com paginação (algorítmo do tipo relógio)

//Bliblioteca
#include <stdio.h>
#include <stdlib.h>

//Definição dos parâmetros globais do gerenciador
#define TAM_PAGINA 4
#define MEM_RAM 4
#define MEM_VIRTUAL 8
int andarilho = 0;

//Elemento da fila de processos
//int uso - Indica se foi utilizado recentemente
//int indice - Guarda o índice de seu quadro de alocação em memória
//char texto - arquivo de texto atribuido a essa página de memoria
struct pagina{
    int uso;
    int indice;
    char texto[TAM_PAGINA];
};

//Indicador da fila de processos
//int pid - identidade do processo
//int tamanho - tamanho em bytes do processo
//struct pagina *inicio - ponteiro para a lista de páginas de memória do processo
//struct processo *prox - ponteiro para o proximo processo da lista
struct processo{
    int pid;
    int tamanho;
    struct pagina *inicio;
    struct processo *prox;
};
struct processo *fila;

//Fila de uso da memória
//int livre - indice do primeiro quadro de memoria livre, se houver
//struct pagina **ponteiro - ponteiro para um vetor de ponteiros de páginas de memória, onde estará armazenado
//as páginas alocadas em meoria principal ou secundária
struct memoria{
    int livre;
    struct pagina **ponteiro;
};
struct memoria ram, virt;



//Funções do programa
void criar(struct processo **lista, int id, int tam); //Cria um processo e suas páginas no tamanho indicado (Função "C")
void trocar(struct pagina *aux); //troca uma página na memoria secundária para a memória principal pelo método do relógio
void escrever(struct processo **lista, int id, int b_set); //encontra, coloca em memória principal, caso ñ estivesse, e escreve no endereço lógico (Função "W")
void ler(struct processo **lista, int id, int b_set); //encontra, coloca em memória principal, caso ñ estivesse, e lê no endereço lógico (Função "R")
void imprimir(struct processo **lista, int id); //Exibe a tabela de alocação de um processo ou o uso da memória (Função "A")


int main()
{
    ram.livre = 0;
    virt.livre = 0;
    ram.ponteiro = (struct pagina **)calloc(MEM_RAM, sizeof(struct pagina*));
    if(ram.ponteiro == NULL){
        printf("Erro na alocacao de memoria das estruturas do gerenciador\n");
        return -1;
    }
    virt.ponteiro = (struct pagina **)calloc(MEM_VIRTUAL, sizeof(struct pagina*));
    if(virt.ponteiro == NULL){
        printf("Erro na alocacao de memoria das estruturas do gerenciador\n");
        return -1;
    }fila = NULL;
    char *leitura;
    leitura = (char *)calloc(10,sizeof(char));
    if(leitura == NULL){
        printf("Erro ao inicializar o gerenciador\n");
        return -1;
    }
    char funcao;
    int dado1;
    int dado2;

    printf("Bem-vindo ao simulador de memoria do grupo 14\n\nTamanho do quadro de memoria: %d bytes\n", TAM_PAGINA);
    printf("Tamanho da Principal: %d paginas (%d bytes)\n", MEM_RAM, (MEM_RAM*TAM_PAGINA));
    printf("Tamanho da Secunaria: %d paginas (%d bytes)\n\n", MEM_VIRTUAL, (MEM_VIRTUAL*TAM_PAGINA));

    while(1){
        //Digite a função que deseja (letra maiuscula associada) e aperte enter
        //Para C, digite o ID do processo e aperte enter, após, numero de quadros e enter
        //Para W e R, digite o ID do processo e aperte enter, após, numero do endereço
        //Para P e I digite o ID do processo e aperte enter, após, digite os identificadores especificos
        //Para A (imprimir) digite o ID do processo e aperte enter (ID = 0 -> panorama da memória)
        fgets(leitura,10,stdin);
        funcao = (char)leitura[0];
        switch (funcao){
            case 'C': //Criar
                fgets(leitura, 10, stdin);
                dado1 = atoi(leitura);
                fgets(leitura, 10, stdin);
                dado2 = atoi(leitura);
                if(dado2 > (MEM_RAM - ram.livre + MEM_VIRTUAL - virt.livre)) printf("Espaço insuficiente em memoria, aguarde!\n");
                else criar(&fila, dado1, dado2);
                break;
            case 'W': //Escrever
                fgets(leitura, 10, stdin);
                dado1 = atoi(leitura);
                fgets(leitura, 10, stdin);
                dado2 = atoi(leitura);
                escrever(&fila, dado1, dado2);
                break;
            case 'R': //Ler
                fgets(leitura, 10, stdin);
                dado1 = atoi(leitura);
                fgets(leitura, 10, stdin);
                dado2 = atoi(leitura);
                ler(&fila, dado1, dado2);
                break;
            case 'P'://Processo em CPU
                fgets(leitura, 10, stdin);
                dado1 = atoi(leitura);
                fgets(leitura, 10, stdin);
                dado2 = atoi(leitura);
                printf("O processo %d realizou uma operacao do tipo %d na CPU\n\n",dado1, dado2);
                break;
            case 'I'://Processo I/O
                fgets(leitura, 10, stdin);
                dado1 = atoi(leitura);
                fgets(leitura, 10, stdin);
                dado2 = atoi(leitura);
                printf("O processo %d realizou uma operacao de entrada/saida com o dispositivo %d\n\n",dado1, dado2);
                break;
            case 'A': //Imprimir
                fgets(leitura, 10, stdin);
                dado1 = atoi(leitura);
                imprimir(&fila, dado1);
                break;
            case 'X': //Sair
                return 0;
            default:
                printf("Opcao invalida\n");
        }
    }
}


//Criar um processo (Função "C")
//struct processo **lista - lista de processos
//int id - identidade do processo a ser criado
//int tam - quantidade de quadros necessário
void criar(struct processo **lista, int id, int tam){

    struct processo *aux;
    aux = *lista;

//Cria todas as paginas de memoria utilizado em um processo e adiciona esse processo a lista de processos
    if(aux == NULL){
        aux = (struct processo *)malloc(sizeof(struct processo));
        if(aux == NULL){
            printf("Erro na alocacao de memoria das estruturas do gerenciador\n");
            return;
        }
        *lista = aux;
    }else{
        while(aux->prox != NULL){
            aux = aux->prox;
        }aux->prox = (struct processo *)malloc(sizeof(struct processo));
        if(aux->prox == NULL){
            printf("Erro na alocacao de memoria para o processo\n");
            return;
        }
        aux = aux->prox;
    }
    aux->pid = id;
    aux->tamanho = tam;
    aux->prox = NULL;

//Criando as páginas de memoria do processo e alocando em RAM
    aux->inicio = (struct pagina *)calloc(tam, sizeof(struct pagina));
    if(aux->inicio == NULL){
            printf("Erro na alocacao de memoria para as paginas do processo\n");
            return;
    }for(int i = 0; i < tam; i++){
        if(ram.livre < MEM_RAM){
            ram.ponteiro[ram.livre] = &aux->inicio[i];
            ram.livre++;
            aux->inicio[i].indice = i;
        }else{
            while(ram.ponteiro[andarilho]->uso){
                ram.ponteiro[andarilho]->uso = 0;
                andarilho = (andarilho + 1) % MEM_RAM;
            }
            virt.ponteiro[virt.livre] = ram.ponteiro[andarilho];
            virt.ponteiro[virt.livre]->uso = -1;
            virt.ponteiro[virt.livre]->indice = virt.livre;
            virt.livre++;
            ram.ponteiro[andarilho] = &aux->inicio[i];
            andarilho = (andarilho + 1) % MEM_RAM;
            aux->inicio[i].indice = andarilho;
        }
        aux->inicio[i].uso = 0;
        for(int j=0; j<TAM_PAGINA; j++) aux->inicio[i].texto[j] = '0';
    }
    return;
}

//Realiza a paginação por meio da estrategia do relógio
//struct pagina *aux - ponteiro para a página de um processo, que será alocada
void trocar(struct pagina *aux){

    while(ram.ponteiro[andarilho]->uso){
        ram.ponteiro[andarilho]->uso = 0;
        andarilho = (andarilho + 1) % MEM_RAM;
    }virt.ponteiro[aux->indice] = ram.ponteiro[andarilho];
    virt.ponteiro[aux->indice]->uso = -1;
    virt.ponteiro[aux->indice]->indice = aux->indice;
    ram.ponteiro[andarilho] = aux;
    ram.ponteiro[andarilho]->uso = 0;
    ram.ponteiro[andarilho]->indice = andarilho;
    andarilho = (andarilho + 1) % MEM_RAM;
    return;
}

//Escrever em um endereço lógico (Função "W")
//struct processo **lista - lista de processos
//int id - identidade do processo a ser escrito
//int b_set - byte a ser escrito
void escrever(struct processo **lista, int id, int b_set){
    struct processo *aux;
    aux = *lista;
    int num_quadro = (b_set - (b_set % TAM_PAGINA))/TAM_PAGINA;
    while(aux != NULL){
        if(aux->pid == id){
            if(aux->tamanho > num_quadro){
                if(aux->inicio[num_quadro].uso == -1) trocar(&aux->inicio[num_quadro]); //Caso a página não esteja em memória principal realiza a paginação
                if(aux->inicio[num_quadro].uso == 0) aux->inicio[num_quadro].uso = 1;
                (aux->inicio[num_quadro]).texto[b_set%TAM_PAGINA] = '1';
                return;
            }else{
                printf("o processo eh menor do que o informado\n");
                return;
            }
        }
        aux = aux->prox;
    }
    printf("Esse processo nao existe em memoria\n");
    return;
}

//Ler um endereço lógico (Função "R")
//struct processo **lista - lista de processos
//int id - identidade do processo a ser escrito
//int b_set - byte a ser escrito
void ler(struct processo **lista, int id, int b_set){
    struct processo *aux;
    aux = *lista;
    int num_quadro = (b_set - (b_set % TAM_PAGINA))/TAM_PAGINA;
    while(aux != NULL){
        if(aux->pid == id){
            if(aux->tamanho > num_quadro){
                if(aux->inicio[num_quadro].uso == -1) trocar(&aux->inicio[num_quadro]); //Caso a página não esteja em memória principal realiza a paginação
                printf("Caracter lido: %c\n", (aux->inicio[num_quadro]).texto[b_set%TAM_PAGINA]);
                return;
            }else{
                printf("o processo eh menor do que o informado\n");
                return;
            }
        }
        aux = aux->prox;
    }
    printf("Esse processo nao existe em memoria\n");
    return;

}

//Imprimir a tabela de atribuição de memoria de um processo ou o uso da memória (Função "A")
//struct processo **lista - lista de processos
//int id - define qual processo terá sua tabela exibida (caso id = 0, exibe a disposição de dados na memória)
void imprimir(struct processo **lista, int id){
    struct processo *aux;
    aux = *lista;

    if(id == 0){ //Exibe o uso e os dados na memória
        printf("Uso da Memoria Principal:\n\n\n");
        for(int i = 0; i < ram.livre; i++){
            printf("Quadro: %d     Dado:", i);
            for(int j = 0; j < TAM_PAGINA; j++) printf("%c",ram.ponteiro[i]->texto[j]);
            if(ram.ponteiro[i]->uso)  printf("     Segunda chance: sim\n");
            else printf("     Segunda chance: nao\n");
        }printf("\n\n\n");
        printf("Uso da Memoria Secundaria:\n\n\n");
        for(int i = 0; i < virt.livre; i++){
            printf("Quadro: %d     Dado:", i);
            for(int j = 0; j < TAM_PAGINA; j++) printf("%c",virt.ponteiro[i]->texto[j]);
            printf("\n");
        }printf("\n\n");
        return;
    }else{ //Exibe as atribuições de páginas de um processo id
        while(aux != NULL){
            if(aux->pid == id){
                printf("Mapa de alocacao do Processo %d\n", id);
                printf("Quadros alocados: %d  Tamanho: %d bytes\n\n", aux->tamanho, aux->tamanho*TAM_PAGINA);
                for(int i = 0; i < aux->tamanho; i++){
                    printf("Pagina do Processo: %d  ", i);
                    if((aux->inicio[i]).uso == -1) printf("em Memoria Secundaria quadro %d\n", (aux->inicio[i]).indice);
                    else printf("em Memoria Principal quadro %d\n", (aux->inicio[i]).indice);
                    printf(" (Dados na pagina: ");
                    for(int j=0; j < TAM_PAGINA; j++) printf("%c", aux->inicio[i].texto[j]);
                    printf(")\n");
                }return;
            }
            aux = aux->prox;
        }
        printf("Esse processo nao existe em memoria\n");
        return;
    }
}
