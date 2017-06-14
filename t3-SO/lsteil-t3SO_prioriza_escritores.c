#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define QTLeitores 3
#define QTEscritores 2

sem_t rmutex; //controla ao acesso para leitores a RC(binário)
sem_t wmutex; //controla o acesso para escritores
sem_t db;    //controla o acesso a base da dados
sem_t readTry; //controla quantidade de tentando ler
sem_t mutex;

int rc = 0;  //número de leitores lendo ou querendo ler
int wc = 0;   //número de escritores pensando ou escrevendo  


void *leitor(void *arg);
void *escritor(void *arg);
void le_db(int i);
void usa_dado(int i);
void escreve(int i);
void pensa(int i);

int main (void){
	//iniciar sem
	sem_init(&rmutex, 0, 1);
	sem_init(&wmutex, 0, 1);
	sem_init(&db, 0, 1);
	sem_init(&readTry, 0, 1);
	sem_init(&mutex, 0, 1);

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
	int ind = *(int*) arg;

	while(1){
		sem_wait(&mutex);
		sem_wait(&readTry);
		sem_wait(&rmutex);
		rc += 1;

		if(rc == 1){
			sem_wait(&db);
		}

		sem_post(&rmutex);
		sem_post(&mutex);
		sem_post(&readTry);

		le_db(ind); //acesso a area crítica
		
		sem_wait(&rmutex);
		rc -= 1 ;

		if(rc == 0){
			sem_post(&db); //só da up no db quando n tem mais nenhum leitor na rc
		}

		sem_post(&rmutex);
		usa_dado(ind);

	}
}

void *escritor(void *arg){
	int ind = *(int*) arg;

	while(1){
		pensa(ind); //fora da rc
		/*pode perder o processador aqui e como isso dar a impressão de erro, pois printou o resultado de pensa
		porém não reservou a seção*/
		sem_wait(&wmutex); //reserva a seção de saída para evitar condição de corrida com leitores
		wc +=1;

		if(wc == 1){
			sem_wait(&readTry);
		}

		sem_post(&wmutex);
		sem_wait(&db); //n permite que outros escritores acessem a base
		escreve(ind);
		sem_post(&db); //libera a base

		sem_wait(&wmutex);
		wc -=1;

		if(wc == 0){
			sem_post(&readTry);
		}

		sem_post(&wmutex);
	}
}

void pensa(int i){
    printf("Sou a thread escritora %d e estou pensando no que escrever(antes de entra na RC)\n", i + 1);
    //sleep(rand() % 3);
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

