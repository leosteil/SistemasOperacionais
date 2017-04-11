/*Trabalho número 1 da disciplina de Sistemas Operacionais*/
/*O trabalho consiste na comparação da criação de processos através da chamada de sistema fork
(ambiente Linux) formando estruturas de árvore binária cheia e cadeia de processos. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void main(int argc, char *argv[]){

	/*entrada inconsistente*/
	if(atoi(argv[1]) <= 0){
		exit(-1);
	}else
		printf("HELLO %d\n", atoi(argv[1]));
} 