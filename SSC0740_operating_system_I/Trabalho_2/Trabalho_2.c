//Login: gso14
//Trabalho de Sistemas Operacionais - Produtor X Consumidor
//Programa com processos consumidores e produtores que controla, por threads de semáforo, as regios críticas

//Bliblioteca
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Buffer limitado global (disponível a todos os processos)
int BUFFER[20];
int espaco_livre = 20;

//Variáveis de sinal para saber se um processo esta em execução
int espera_produtor = 0;
int espera_consumidor = 0;
int fim = 0;

//Semáforos de controle das threads
pthread_mutex_t buffer_control = PTHREAD_MUTEX_INITIALIZER; //Controla o uso do buffer
pthread_mutex_t thread_control = PTHREAD_MUTEX_INITIALIZER; //Controla a execução de uma thread 
pthread_cond_t libera = PTHREAD_COND_INITIALIZER;           //Controla quando uma thread deve acordar

//Elemento da fila de memoria disponível
//int indice - armazena o indice do buffer
//struct fila *prox - endereço do próximo elemento da fila
struct fila{
    int indice;
    struct fila *prox;
};

//Struct de ponteiro que armazena os endereços inicial de cada fila
//struct fila *entrada - armazena o endereço do primeiro elemento da fila de buffer preenchidos
//struct fila *fim - armazena o endereço do primeiro elemento da fila de buffer vazio
struct ref_fila{
    struct fila *entrada;
    struct fila *saida;
};

//declara e inicia a struct ref_fila para uso nas funções
struct ref_fila inicio, final;

//Funções do programa
void cria_lista();                         //Cria e inicializa as listas de buffer livre e cheio
void troca_elementos(int n);               //Transfere o primeiro elemento da fila 1 para o final da fila 2
void produz_elemento(int dado);            //Escreve no buffer
int consome_elemento();                    //Lê o buffer
void *produtor();                          //Função produtor 
void *consumidor();                        //Função consumidor

int main(){

    pthread_t t1; //Thread do processo produtor
    pthread_t t2; //Thread do processo consumidor

    srand(time(NULL));//Para criar o valor aleatorio
    cria_lista(); //Cria e inicializa as listas de buffer livre e cheio
    printf("\nLeitura dos dados do Buffer:\n\n");
    if(pthread_create(&t1, NULL, produtor, NULL)){   //Inicia e testa o processo produtor
        fprintf(stderr,"ERRO - pthread_create()");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&t2, NULL, consumidor, NULL)){  //Inicia e testa o processo consumidor
        fprintf(stderr,"ERRO - pthread_create()");
        exit(EXIT_FAILURE);
    }
    //Espera a conclusão das threads
    pthread_join(t1, NULL);
    printf("\n> > > Termino do processo produtor! < < <\n\n");
    pthread_join(t2, NULL);
    printf("\n> > > Termino do processo consumidor! < < <\n\n");
    return 0;
}

//Cria e inicializa as listas de buffer livre e cheio
void cria_lista(){

    //Crindo a fila de entrada (buffer livre)
    inicio.entrada = (struct fila*)malloc(sizeof(struct fila));
    if(inicio.entrada == NULL){
        printf("Não foi possivel alocar memoria para a lista de buffer\n");
        return;
    }
    inicio.entrada->indice = 0;
    inicio.entrada->prox = NULL;
    final.entrada = inicio.entrada;
    for(int i = 1; i < 20; i++){
        final.entrada->prox = (struct fila*)malloc(sizeof(struct fila));
        if(final.entrada->prox == NULL){
            printf("Não foi possivel alocar memoria para a lista de buffer\n");
            return;
        }final.entrada = final.entrada->prox;
        final.entrada->indice = i;
        final.entrada->prox = NULL;
    }
    //Inicia o primeiro elemento da lista de saida
    inicio.saida = NULL;
    final.saida = NULL;

    return;
}


//Transfere o primeiro elemento da fila 1 para o final da fila 2
//int n - determina qual o metodo a ser realizado
void troca_elementos(int n){

    if(n == 0){//Retira da lista de entrada e adiciona a lista de saida
        (final.saida == NULL) ? (inicio.saida = inicio.entrada) : (final.saida->prox = inicio.entrada);
        final.saida = inicio.entrada;
        inicio.entrada = inicio.entrada->prox;
        if(inicio.entrada == NULL) final.entrada = NULL;
        final.saida->prox = NULL;
    }else{//Retira da lista de saida e adiciona a lista de entrada
        (final.entrada == NULL) ? (inicio.entrada = inicio.saida) : (final.entrada->prox = inicio.saida);
        final.entrada = inicio.saida;
        inicio.saida = inicio.saida->prox;
        if(inicio.saida == NULL) final.saida = NULL;
        final.entrada->prox = NULL;
    }

    return;
}


//Escreve no buffer
//int dado - dado a ser escrito no buffer
void produz_elemento(int dado){

    //Escreve na primeira posição da lista de entrada
    BUFFER[(inicio.entrada)->indice] = dado;
    //Elimina esse elemento da lista de entrada e passa-o para a lista de saida
    troca_elementos(0);
    //Diminui o contador de espaçoes livres do buffer
    espaco_livre--;
    return;
}

//Lê o buffer
//return - retorna o dado lido no buffer
int consome_elemento(){
    //Le na primeira posição da lista de saida
    int dado = BUFFER[(inicio.saida)->indice];
    //Elimina esse elemento da lista de saida e passa-o para a lista de entrada
    troca_elementos(1);
    //Aumenta o contador de espaçoes livres do buffer
    espaco_livre++;
    return dado;
}

//Função produtor 
void *produtor(){

    int dado = (rand() % 13);  //Variável dos dados a ser escrita no buffer
  
   while(dado < 200){
        //Verifica se tem espaço livre e, caso não tenha, coloca o processo em pausa
        pthread_mutex_lock(&thread_control);
        if(espaco_livre == 0){
            espera_produtor = 1;
            while(espera_produtor) espera_produtor = pthread_cond_wait(&libera, &thread_control);
        }pthread_mutex_unlock(&thread_control);

        //Escreve no buffer
        pthread_mutex_lock(&buffer_control);
        produz_elemento(dado);
        pthread_mutex_unlock(&buffer_control);

        //Verifica se o processo consumidor está em pausa e libera-o
        pthread_mutex_lock(&thread_control);
        if(espera_consumidor == 1) pthread_cond_signal(&libera);
        pthread_mutex_unlock(&thread_control);
       
        //Altera aleatoriamente o valor do dado
        dado  += (rand() % 13);
    }

    //Uma vez que todos os dados do produtor sejam escritos no buffer, um sinal fim é determinado para controle do consumidor
    fim = 1;
}

//Função consumidor
void *consumidor(){

    int dado;       //Variável dos dados a ser lida no buffer

    while(1){

        //Verifica se tem conteudo a ser consumido no buffer e, caso não tenha, coloca o processo em pausa
        pthread_mutex_lock(&thread_control);
        if(espaco_livre == 20){
            if(fim) return; //Verifica se o sinal de termino do produtor foi acionado, caso afirmativo, encerra sua execução
            espera_consumidor = 1;
            while(espera_consumidor) espera_consumidor = pthread_cond_wait(&libera, &thread_control);
        }pthread_mutex_unlock(&thread_control);

        //Lê o buffer
        pthread_mutex_lock(&buffer_control);
        dado = consome_elemento();
        pthread_mutex_unlock(&buffer_control);

        //Verifica se o processo produtor está em pausa e libera-o
        pthread_mutex_lock(&thread_control);
        if(espera_produtor == 1) pthread_cond_signal(&libera);
        pthread_mutex_unlock(&thread_control);

        //Exibe na tela o dado lido no buffer
        printf("Novo dado lido: %d\n", dado);

    }    
}
