#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

typedef struct{
	int numero;
	int estado; // 1-disponível, 0-ocupado, -1-reservado(em compra)
	int dono;
}Lugar;

typedef struct{
	int ind;	
	int nIngressos;
	//Lugar *lugares;
}ParametroTh;

Lugar *lugares; // inicia o vetor com 0 posições 
int compradores = 0; //controla o numero de compradores
sem_t mutex; //controla o acesso de apenas uma thread compradora, pq só tem 1 vendedo

void verificaEntrada(int entrada);
void preencheVetorDeLugares(Lugar l[] , int max);

/*No caso deste trabalho, o primeiro comprador tem direito ao primeiro acento
e assim sucessivamente. Logo essa função retorna a primeira posição livre do vetor*/	
int selecionaPosicao(int ind, int tam);
int transacao(void);
int semCompradores(void);
void situacaoFinal(int tam);

void *compraIngresso(void* arg);

int main(int argc, char *argv[]){
	srand(time(NULL));
	
	if(!(argv[1])){
		printf("Deve ser passado uma quantidade máxima de lugares para o evento\n");
        exit(-1);
	}else verificaEntrada(atoi(argv[1]));

    sem_init(&mutex, 0, 1);

	/*criação do vetor de lugares + preenchimento*/
	lugares = malloc(sizeof(Lugar) * atoi(argv[1]));
    preencheVetorDeLugares(lugares, atoi(argv[1]));


    pthread_t threadsCompradoras[atoi(argv[1])];
    ParametroTh parCompradora[atoi(argv[1])]; //parametro da thread, nesse caso só o indice da thread

    //gera threads compradoras
    for(int i = 0; i < atoi(argv[1]); i++){
        parCompradora[i].ind = i + 1;
        parCompradora[i].nIngressos = atoi(argv[1]);
        pthread_create(&threadsCompradoras[i], NULL, compraIngresso, (void*) &parCompradora[i]);
    }

    // join nas threads
    for(int i = 0; i < atoi(argv[1]); i++){
        pthread_join(threadsCompradoras[i], NULL);
    }

    situacaoFinal(atoi(argv[1]));

    free(lugares);
    sem_destroy(&mutex);

	return 0;
}

void verificaEntrada(int entrada){
	if(!(entrada)){
        printf("Deve ser passado uma quantidade máxima de lugares para o evento\n");
        exit(-1);
    }else if((entrada) <= 0){
        printf("O argumento deve ser um valor maior que zero\n");
        exit(-1);
    }
}

void preencheVetorDeLugares(Lugar l[], int max){
	int i;

	for (i = 0; i < max; i++){
		l[i].numero = i+1;
		l[i].estado = 1 ;
		l[i].dono = 0;
	}
}

int selecionaPosicao(int ind, int tam){
	sem_wait(&mutex);
	printf("sou a thread %d e estou escolhendo\n", ind);
	int x = 0;
	int flag = 0;
	while(x < tam){
		if(lugares[x].estado == 1){
			flag = x;
			break;
		}
		x++;
	}

	lugares[flag].estado = -1; //impossibilita a seleção do mesmo local

	sem_post(&mutex);
	printf("sou a thread %d e escolhi o lugar %d\n", ind, flag + 1);
	return flag;
}

/*valores para a transação*/
int transacao(void){
	sleep(rand()%2);

	int x = rand() % 3;
	if(x == 0 || x == 1){
		return 1; //aprovada
	}

	return 0;
}

void *compraIngresso(void* arg){
	ParametroTh p = *(ParametroTh*) arg; //thread - indice da thread
	int pos = 0;

	pos = selecionaPosicao(p.ind, p.nIngressos); 

	if(transacao()){
		lugares[pos].estado = 0;
		lugares[pos].dono = p.ind;
		printf("Thread %d conseguiu comprar o lugar %d\n", p.ind, pos + 1);
	}else{
		lugares[pos].estado = 1;
		printf("Thread %d tentou comprar o lugar %d e não conseguiu\n", p.ind, pos + 1);
	} 

	compradores+=1;

	return EXIT_SUCCESS;
}

/*Função utilizada para mostrar a situação final do vetor de lugares*/
void situacaoFinal(int tam){
	for (int i = 0; i < tam; ++i){
    	printf("Lugar:%d Estado:%d Thread Dona:%d\n", lugares[i].numero, lugares[i].estado, lugares[i].dono);
    }

}