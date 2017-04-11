/* arquivo ex3-fork */
/* resultados do fork comparados através de um switch*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

	pid_t idProcesso;
	printf("Iniciando o programa\n");

	idProcesso = fork();

	switch (idProcesso){
		case -1: exit(-1);

		case 0: printf("Sou um processo filho, meu id eh = %d, meu pai eh %d\n", getpid(), getppid());
				break;		

		default: printf("Sou o processo pai %d, meu pai eh %d e meu filho eh %d\n", getpid(), getppid(), idProcesso);

	}
}