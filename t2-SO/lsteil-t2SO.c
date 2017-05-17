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
#include <pthreads.h>


typedef struct parametrosThreads{
	int numThread = 0;
	int *vet;
}Parametros;

/*função usada para preencher vetor com número aleatórios*/
/*o range vai até 500*/
int* preencheVetor(int tam_vetor, int *vetor){

	vetor = (int *) malloc(tam_vetor*sizeof(int));

	srand(time(NULL));
	for (int i = 0; i < tam_vetor; i++){
		vetor[i] = rand() % 500;
		printf("%d ", vetor[i]);
	}
	printf("\n");
	return vetor;
}

int verificaPrimo(int valor, int div){
	int teste = 1;

	if(valor % div == 0){
		teste =  0;
	}

	if(teste == 0){
		return 0;
	}else return 1;
}

void buscaSequencial(int tam_vetor, int *vetor){
	/*se teste = 0 número não primo, else if teste = 1 número primo*/

	int div = 0;
	int teste, limite, primos =0;

	for (int i = 0; i < tam_vetor; i++) {
  		if(vetor[i] > 1){
  			teste = 1;
  			div = 2;
  			limite = sqrt(vetor[i]);;
  			while(teste == 1 && div < limite){
  				if((vetor[i] % div) == 0){
  					teste = 0;
  				}
  				div ++;
  			}
  			if(teste){
  				primos ++;
  			}
  		}
  	}

  	printf("Sequencial -> %d\n", primos);
}

void buscaProcessos(int tam_vetor, int *vetor, int qntdProcessos){

}

void buscaThreads(int tam_vetor, int *vetor, int qntdThreads, pid_t pai){
	pthreads threads[qntdThreads];
	Parametros p[qntdThreads];

	for(int i = 0; i < qntdThreads; i++){
		p[i].numThread = i;
		p.[i].vetor = vetor;
		pthread_create(&threads[i], NULL, percorreTrecho, (void*) &args[i]);
	}

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
}

void percorreTrecho()


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
}