/* arquivo ex1-testa_id */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	pid_t idPai;
	pid_t idProcesso;
	idPai = getppid();
	idProcesso = getpid();

	printf("PID = %d, PPD = %d\n", idProcesso, idPai);

	exit(0);	
}
