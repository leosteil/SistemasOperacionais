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

double difTime(struct timespec t0, struct timespec t1){
    return ((double)t1.tv_sec - t0.tv_sec) + ((double)(t1.tv_nsec-t0.tv_nsec) * 1e-9);
}

struct parametrosThreads{
	int numThread;
	int *vet;
	int finishPoint;
	int startPoint;
};

struct retornoThread{
	int qtdPrimos;
	int numThread;
};

struct retornoProcesso{
	int pid;
	int qtdPrimos;
};

/*função usada para preencher vetor com número aleatórios*/
/*o range vai até 500*/
int* preencheVetor(int tam_vetor, int *vetor){

	vetor = (int *) malloc(tam_vetor*sizeof(int));

	srand(time(NULL));
	for (int i = 0; i < tam_vetor; i++){
		vetor[i] = rand() % 3000;
		printf("%d ", vetor[i]);
	}
	printf("\n");
	return vetor;
}

int verificaPrimo(int valor){
	int teste = 1;
	int limite = sqrt(valor) + 1;

	if(valor % 2 == 0){
		teste = 0;
	}else{
		for(int i = 3; i < limite; i = i + 2){
			if((valor % i) == 0){
			teste = 0;
		}
		}
	}

	if(teste){
		return 1;
	}else return 0;
}

void buscaSequencial(int tam_vetor, int *vetor, FILE *arq){
	struct timespec t0, t1;
	int cont = 0, contadorDePrimos = 0;

	clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
	for (int i = 0; i < tam_vetor; i++) {
  		if(vetor[i] > 1){
  			cont = verificaPrimo(vetor[i]);
  			contadorDePrimos = contadorDePrimos + cont;
  		}
  	}

  	printf("Sequencial -> %d\n", contadorDePrimos);
  	clock_gettime(CLOCK_MONOTONIC_RAW, &t1);    
    fprintf(arq, "%f\n", difTime(t0,t1));
  	
}

int buscaProcessos(int tam_vetor, int *vetor, int qntdProcessos, int pidRaiz, FILE * arq){
	struct timespec t0, t1;
	pid_t child;
	int fd[2]; /*file descriptor pro Pipe*/

	clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
	for(int i = 0; i < qntdProcessos; i++){
		
		if(pidRaiz == getpid()){

			if(pipe(fd)<0){
				perror("pipe");
					return -1;
			}

			if((child = fork()) < 0){
				perror("fork");
				exit(1);
			}

			if(child == 0){

				int fim = 0, ini = 0;
				ini = i * (tam_vetor / qntdProcessos);

				if(i == qntdProcessos - 1){
					fim = tam_vetor;
				}else fim = ini + (tam_vetor / qntdProcessos);

				struct retornoProcesso ret;
				close(fd[0]);

				int retorno = 0; 
				ret.pid = getpid();
				
				for(int i = ini; i < fim; i++){
					if(vetor[i] > 1){
						retorno += verificaPrimo(vetor[i]);
					}
				}	
				ret.qtdPrimos = retorno;

				write(fd[1], &ret, sizeof(struct retornoProcesso));
				retorno = 0;	
				exit(1);
			}else if (child > 0){
				wait(0);
				close(fd[1]);
				struct retornoProcesso ret;
				read(fd[0], &ret, sizeof(struct retornoProcesso));
				printf("Processo PID->%d, Primos=%d\n", ret.pid, ret.qtdPrimos);
					
			}
		}
	}

	clock_gettime(CLOCK_MONOTONIC_RAW, &t1);    
    fprintf(arq, "%f\n", difTime(t0,t1));
}


void *percorreTrecho(void *p) {
	struct parametrosThreads args = *(struct parametrosThreads*) p;
	int contadorDePrimos = 0, cont = 0;
	struct retornoThread *ret = malloc(sizeof(struct retornoThread));
	for(int i = args.startPoint; i < args.finishPoint; i++){
		if(args.vet[i] > 1){
			cont = verificaPrimo(args.vet[i]);
			contadorDePrimos += cont;
		}
	}

	ret->numThread = args.numThread;
	ret->qtdPrimos = contadorDePrimos;

	return (void*)ret;
}

void buscaThreads(int tam_vetor, int *vetor, int qntdThreads, FILE * arq){
	struct timespec t0, t1;
	pthread_t threads[qntdThreads];
	struct parametrosThreads p[qntdThreads];
	struct retornoThread *ret[qntdThreads];

	clock_gettime(CLOCK_MONOTONIC_RAW, &t0); 
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

	for(int i = 0; i < qntdThreads; i++){
		pthread_join(threads[i], (void*)&ret[i]);
		printf("Thread %d -> %d primos\n", ret[i]->numThread+1, ret[i]->qtdPrimos);
	}

	clock_gettime(CLOCK_MONOTONIC_RAW, &t1);    
   	fprintf(arq, "%f\n", difTime(t0,t1));
}


int main(int argc, char *argv[]){

    /*controles de entrada*/
    if(!(argv[1])){
        printf("Deve ser passado um tamanho para o vetor\n");
        exit(-1);
    }else if(atoi(argv[1]) < 0){
        printf("O argumento deve ser um valor maior que zero\n");
        exit(-1);
    }

    if(!(argv[2])){
        printf("Deve ser passado uma quantidade de processos\n");
        exit(-1);
    }else if(atoi(argv[2]) < 0){
        printf("O argumento deve ser um valor maior que zero\n");
        exit(-1);
    }

    /*controle de entradas*/

    int *vetor;
   	vetor = preencheVetor(atoi(argv[1]),vetor);

    FILE *fp, *fp1, *fp2;

    fp = fopen("ResultadosSEQ.txt", "a");

    if (fp == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       exit(0);
    }

    buscaSequencial(atoi(argv[1]), vetor, fp);
    fclose(fp);

    printf("------------------------------\n");

    fp1 = fopen("ResultadosThreads.txt", "a");

    if (fp1 == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       exit(0);
    }

    buscaThreads(atoi(argv[1]), vetor, atoi(argv[2]), fp1);
    fclose(fp1);

    printf("------------------------------\n");

    fp2 = fopen("ResultadosProcessos.txt", "a");

    if (fp2 == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       exit(0);
    }

    buscaProcessos(atoi(argv[1]), vetor, atoi(argv[2]), getpid(), fp2);
    fclose(fp2);

    printf("------------------------------\n");

   	return 0;
}