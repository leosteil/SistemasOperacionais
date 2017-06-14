#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define QTLeitores 3
#define QTEscritores 2


int rc;

//semaphores
sem_t db; //controle de acesso a base de dados
sem_t serviceQueue; //preserva a ordem das requisições7
sem_t readCountAccess; // sincronização de mudanças variavel compartilhada readCount;


void *leitor(void *arg);
void *escritor(void *arg);
void le_db(int i);
void usa_dado(int i);
void escreve(int i);
void pensa(int i);


int main (void){
    //iniciar sem
    sem_init(&serviceQueue, 0, 1);
    sem_init(&db, 0, 1);
    sem_init(&readCountAccess, 0, 1);

    //vetores de threads
    pthread_t threadsLeitoras[QTLeitores];
    pthread_t threadsEscritoras[QTEscritores];

    int parLeitores[QTLeitores];
    int parEscritores[QTEscritores];

    //gera threads leitores
    for(int i = 0; i < QTLeitores; i++){
        parLeitores[i] = i;
        pthread_create(&threadsLeitoras[i], NULL, leitor, (void*) &parLeitores[i]);
    }


    //gera threads escritores
    for(int i = 0; i < QTEscritores; i++){
        parEscritores[i] = i;
        pthread_create(&threadsEscritoras[i], NULL, escritor, (void*) &parEscritores[i]);
    }

    
    //join nas threads

    for(int i = 0; i < QTLeitores; i++){
        pthread_join(threadsLeitoras[i], NULL);
    }

    for(int i = 0; i < QTEscritores; i++){
        pthread_join(threadsEscritoras[i], NULL);
    }

}

void *leitor(void *arg){
    int ind = *(int*) arg; //thread

    while(1){
        
        sem_wait(&serviceQueue);    // espera para ser servido
        sem_wait(&readCountAccess); // requisita acesso esclusivo para apenas leitor

        if(rc == 0){                //se não tem nenhum leitor já lendo
            sem_wait(&db);          //seta o db para os leitores
        }
        rc += 1;                    //aumenta a quantidade de leitores ativos

        sem_post(&serviceQueue);    //permite o proximo da fila ser servido
        sem_post(&readCountAccess); // libera o acesso para o readCount
        
        le_db(ind);             // le a base de dados
        
        sem_wait(&readCountAccess);        // requisita acesso exclusivo a readCountAcess

        rc -= 1;                // update no número de leitores ativos
        if (rc == 0)         // se não tem mais leitores
            sem_post(&db);     // libera acesso total ao db
        
        sem_post(&readCountAccess);        // libera o acesso a readCount
        usa_dado(ind);
    }
}

void *escritor(void *arg){
    int ind = *(int*) arg; //thread

    while(1){
        pensa(ind);

        sem_wait(&serviceQueue); // wait in line to be serviced
        

        sem_wait(&db);           // request exclusive access to resource
        
        sem_post(&serviceQueue); // let next in line be serviced           
    
        escreve(ind);

        sem_post(&db);
    }
}

void pensa(int i){
    printf("Sou a thread escritora %d e estou pensando no que escrever(antes de entra na RC)\n", i + 1);
    sleep(rand() % 3);
}

void escreve(int i){
    printf("Sou a thread escritora %d e estou escrevendo (dentro da RC)\n", i + 1);
    sleep(rand() % 3);
}

void usa_dado(int i){
    printf("Sou a thread %d leitora vou usar os dados (estou fora da RC) \n", i + 1);
    sleep(rand() % 3);
}

void le_db(int i){
    printf("Sou a thread %d leitora e vou ler (dentro da RC) \n", i + 1);
    sleep(rand() % 3);
}