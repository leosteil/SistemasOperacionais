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

/*função usada para preencher vetor com número aleatórios*/
/*o range vai até 500*/
int* preencheVetor(int tam_vetor, int *vetor){

	vetor = (int *) malloc(tam_vetor*sizeof(int));

	srand(time(NULL));
	for (int i = 0; i < tam_vetor; i++){
		vetor[i] = rand() % 500;
	}

	return vetor;
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
}