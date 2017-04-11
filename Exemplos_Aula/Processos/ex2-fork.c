/* arquivo ex2-fork */
/* fork feito com if */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	pid_t idProcesso;

	idProcesso = fork();

	if(idProcesso <0){ //erro no fork
		fprintf(stderr, "fork falhou\n");
	}
	else if(idProcesso == 0) //filho
		printf("Sou o FILHO, meu id =%d, meu pai eh %d\n", getpid(), getppid());
	else
		printf("Sou o PAI, meu id = %d, meu pai eh %d\n", getpid(), getppid());

	exit(0);
}