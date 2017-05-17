/*Trabalho número 2 da disciplina de Sistemas Operacionais*/
/*O trabalho consiste na comparação entre o uso de processos, através da chamada fork (ambiente
Linux) e threads utilizando a biblioteca pthreads, para o cálculo de números primos.*/
/*O programa deve receber dois parâmetros pela linha de comando (usar argc e argv), ambos inteiros
e positivos. O primeiro número representa o tamanho do vetor (N) e o segundo, o número de
threads/processos (M) que serão utilizados para o processamento.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>

struct parametrosThreads{
	int numThread;
	int *vet;
	int finishPoint;
	int startPoint;
};

/*função usada para preencher vetor com número aleatórios*/
/*o range vai até 500*/
int* preencheVetor(int tam_vetor, int *vetor){
	srand(time(NULL));
	for (int i = 0; i < tam_vetor; i++){
		vetor[i] = rand() % 500;
		printf("%d ", vetor[i]);
	}
	printf("\n");
	return vetor;
}

int verificaPrimo(int valor){
	int div = 2;
	int teste = 1;
	int limite = sqrt(valor);
	while(teste == 1 && div < limite){
		if((valor % div) == 0){
			teste = 0;
		}
		div ++;
	}

	if(teste){
		return 1;
	}else return 0;
}

void buscaSequencial(int tam_vetor, int *vetor){

	int cont = 0, contadorDePrimos = 0;

	for (int i = 0; i < tam_vetor; i++) {
  		if(vetor[i] > 1){
  			cont = verificaPrimo(vetor[i]);
  			contadorDePrimos = contadorDePrimos + cont;
  		}
  	}

  	printf("Sequencial -> %d\n", contadorDePrimos);
}

void buscaProcessos(int tam_vetor, int *vetor, int qntdProcessos){

}


void *percorreTrecho(void *p) {
	struct parametrosThreads args = *(struct parametrosThreads*) p;
	int contadorDePrimos = 0, cont = 0;

	for(int i = args.startPoint; i < args.finishPoint; i++){
		if(args.vet[i] > 1){

			cont = verificaPrimo(args.vet[i]);
			contadorDePrimos = contadorDePrimos + cont;
		}
	}

	printf("Thread %d - > %d primos\n", args.numThread, contadorDePrimos);

	return EXIT_SUCCESS;
}

void buscaThreads(int tam_vetor, int *vetor, int qntdThreads){
	pthread_t threads[qntdThreads];
	struct parametrosThreads p[qntdThreads];

	for(int i = 0; i < qntdThreads; i++){
		p[i].numThread = i;
		p[i].vet = vetor;
		
		if(i == 0){
			p[i].startPoint = 0;
		}else p[i].startPoint = p[i - 1].finishPoint;

		if(i == qntdThreads - 1){
			p[i].finishPoint = tam_vetor;
		}else p[i].finishPoint = (tam_vetor / qntdThreads) * (i+1);

		pthread_create(&threads[i], NULL, percorreTrecho, (void*) &p[i]);
	}

	for (int i = 0; i < qntdThreads; i++)
		pthread_join(threads[i], NULL);
}


int main(int argc, char *argv[]){

    /*controles de entrada*/
    if(!(argv[1])){
        printf("Deve ser passado um valor por referência na execução do arquivo\n");
        exit(-1);
    }else if(atoi(argv[1]) < 0){
        printf("O argumento deve ser um valor maior que zero\n");
        exit(-1);
    }
    /*controle de entradas*/

    int *vetor;
   	vetor = preencheVetor(atoi(argv[1]),vetor);

   	buscaSequencial(atoi(argv[1]),vetor);

   	buscaThreads(atoi(argv[1]), vetor, atoi(argv[2]));

   	return 0;
}