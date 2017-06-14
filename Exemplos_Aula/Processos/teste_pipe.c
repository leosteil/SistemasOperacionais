#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER 256

int main(void){
	int fd[2]; /*file descriptor pro Pipe*/
	pid_t pid;

	int *vetor = malloc(sizeof(int)*10);

	for(int i = 0; i < 10; i++){
		vetor[i] = i;
	}

	if(pipe(fd)<0){
		perror("pipe");
		return -1;
	}

	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}

	if(pid > 0){
		close(fd[0]);

		char str[BUFFER] = "Aprendi a usar Pipes em C!";
		printf("String enviada pelo pai no Pipe: %s\n", str);

		write(fd[1], str, sizeof(str) + 1);
		exit(0);
	}else{
		char str_recebida[BUFFER]  = "pEDRO";

		close(fd[1]);

		read(fd[0], str_recebida, sizeof(str_recebida));

		printf("String lida pelo filho no Pipe : %s\n", str_recebida);
		exit(0);
	}

	return 0;
}