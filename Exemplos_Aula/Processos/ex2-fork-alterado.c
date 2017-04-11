#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (){
	pid_t idProcesso;
	idProcesso = fork();

	if(idProcesso < 0){ //erro no fork
		fprintf(stderr, "Fork Falhou\n");
		exit(-1);
	}
	else if(idProcesso == 0) //filho
		printf("sou o FILHO, meu id = %d, meu pai eh %d\n\n", getpid(), getppid());
	else
		printf("sou o PAI, meu id = %d, meu pai eh %d\n\n", getpid(), getppid());

	printf("Sou executado por ambos. Sou %d.\n", getpid());
	exit(0);
}