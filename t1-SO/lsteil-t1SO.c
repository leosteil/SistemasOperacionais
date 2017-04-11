/*Trabalho número 1 da disciplina de Sistemas Operacionais*/
/*O trabalho consiste na comparação da criação de processos através da chamada de sistema fork
(ambiente Linux) formando estruturas de árvore binária cheia e cadeia de processos. */
/*Neste caso, o argv[1] será o tamanho da árvore e a quantidade de processos será dado por 2^(h+1) + 1 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void estruturaCadeia(int qntdProcesso){	
	int i = 0;
	int status;
	pid_t idProcesso;

	for(i == 0; i < qntdProcesso - 1; i++){
		idProcesso = fork();

		if(idProcesso > 0){
			printf("Sou o processo original, meu id = %d, meu pai eh %d\n", getpid(), getppid());
			wait(&status);
			break;
		}

		if(idProcesso <0){ //erro no fork
			fprintf(stderr, "fork falhou\n");
		}
		else if(idProcesso == 0) //filho
			printf("Sou o FILHO, meu id =%d, meu pai eh %d\n", getpid(), getppid());
	}

		printf("Sou o precesso %d e estou saindo\n", getpid());
}

void estrutura (int h){

}


void main(int argc, char *argv[]){

	/*entrada inconsistente*/
	if(atoi(argv[1]) <= 0){
		exit(-1);
	}

	int qntdProcessos = (2 * (atoi(argv[1]) + 1) + 1);
	printf("%d\n", qntdProcessos);

	estruturaCadeia(qntdProcessos);

	
 } 


