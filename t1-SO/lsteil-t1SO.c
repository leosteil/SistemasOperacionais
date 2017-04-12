/*Trabalho número 1 da disciplina de Sistemas Operacionais*/
/*O trabalho consiste na comparação da criação de processos através da chamada de sistema fork
(ambiente Linux) formando estruturas de árvore binária cheia e cadeia de processos. */
/*Neste caso, o argv[1] será o tamanho da árvore e a quantidade de processos será dado por 2^(h+1) + 1 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

void estruturaCadeia(int qntdProcesso, int pidPai){
    int i = 0;

    pid_t idProcesso;
    int estado;

    printf("Sou o processo incial com id %d e meu pai eh %d\n", getpid(), getppid());

    for(i == 0; i <qntdProcesso; i++){
    	idProcesso = fork();

        if(idProcesso == 0){
            printf("Sou o processo %d meu pai eh %d\n", getpid(), getppid());
        }else if(idProcesso <0){ //erro no fork
            fprintf(stderr, "fork falhou\n");
        }
        else{ //pai
            printf("Sou o processo pai %d meu pai eh %d\n", getpid(), getppid());
            wait(0);
            
            //exit(0);
            break;
        }

    }

            printf("Sou o processo %d e estou saindo\n", getpid()); 
    
}


void estruturaArv (int h){
    int i = 0;


    printf("Sou o processo inicial com id %d e meu pai eh %d\n", getpid(), getppid());

    for(i=0; i<h; i++ ){
        if(fork() == 0){
            printf("Sou o processo %d e meu pai eh %d\n", getpid(), getppid());
            continue;
        }
        if(fork() == 0){
            printf("Sou o processo %d e meu pai eh %d\n", getpid(), getppid());
            continue;
        }

        wait(0);        
        break;
    }
        printf("Sou o processo %d e estou saindo meu pai eh%d\n", getpid(), getppid());
}


void main(int argc, char *argv[]){
    clock_t inicio, fim;
    float total;
    /*entrada inconsistente*/
    if(atoi(argv[1]) <= 0){
        exit(-1);
    }

    int qntdProcessos = (2 ^ (atoi(argv[1]) + 1) + 1);

    inicio = clock();
    pid_t programaRaiz = getpid();
    //estruturaCadeia(qntdProcessos, getpid());
    
    estruturaArv(atoi(argv[1]));

    //fim = clock();
    //if (getpid() == programaRaiz)
    //   printf("%f Tempo, %d;\n", (double)(fim - inicio) / CLOCKS_PER_SEC, programaRaiz);
}
