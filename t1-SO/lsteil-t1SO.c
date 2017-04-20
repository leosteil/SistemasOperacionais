/*Trabalho número 1 da disciplina de Sistemas Operacionais*/
/*O trabalho consiste na comparação da criação de processos através da chamada de sistema fork
(ambiente Linux) formando estruturas de árvore binária cheia e cadeia de processos. */
/*Neste caso, o argv[1] será o tamanho da árvore e a quantidade de processos será dado por 2^(h+1) + 1 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
#include <math.h>

double difTime(struct timespec t0, struct timespec t1){
    return ((double)t1.tv_sec - t0.tv_sec) + ((double)(t1.tv_nsec-t0.tv_nsec) * 1e-9);
}

void estruturaCadeia(int h, FILE *arq){
    printf("CADEIA\n");

    int i = 0;
    struct timespec t0, t1;
    int qntdProcessos = pow(2,(h+1))-1;
    printf("%d\n", qntdProcessos);

    pid_t child;
    int pai = getpid();

    printf("Sou o processo incial com id %d e meu pai eh %d\n", getpid(), getppid());

    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
    for(i = 0; i <qntdProcessos - 1; i++){
        child = fork();

        if(child == 0){
            printf("Sou o processo %d meu pai eh %d\n", getpid(), getppid());
        }else if(child < 0){ //erro no fork
            fprintf(stderr, "fork falhou\n");
            exit(-1);
        }
        else{ //pai
            wait(0);
            break;
        }
    }
        if(pai != getpid()){
            printf("Sou o processo %d e estou saindo meu pai eh%d\n", getpid(), getppid());
            exit(1);
        }else if(pai == getpid()){ 
            printf("Sou o processo %d e estou saindo meu pai eh%d\n", getpid(), getppid());
            clock_gettime(CLOCK_MONOTONIC_RAW, &t1);    
            fprintf(arq, "%f\n", difTime(t0,t1));          
        }    
}


void estruturaArv (int h, FILE *arq){
    printf("ARVORE\n");

    int i = 0;
    struct timespec t0, t1;
    int pai = getpid();
    pid_t child1, child2;

    printf("Sou o processo inicial com id %d e meu pai eh %d\n", getpid(), getppid());

    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);    
    for(i=0; i<h; i++ ){
        child1 = fork();
        if(child1 == 0){
            printf("Sou o processo %d e meu pai eh %d\n", getpid(), getppid());
            continue;
        }else if(child1 == -1){
            fprintf(stderr, "fork falhou\n");
            exit(-1);
        }

        child2 = fork();
        if(child2 == 0){
            printf("Sou o processo %d e meu pai eh %d\n", getpid(), getppid());
            continue;
        }else if(child2 == -1){
            fprintf(stderr, "fork falhou\n");
            exit(-1);
        }

        wait(0);
        wait(0);
        break;
    }

        if(pai != getpid()){
            printf("Sou o processo %d e estou saindo meu pai eh%d\n", getpid(), getppid());
            exit(0);
        }else if(pai == getpid()){
            printf("Sou o processo %d e estou saindo meu pai eh%d\n", getpid(), getppid());
            clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
            fprintf(arq, "%f\n", difTime(t0, t1));
        }
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

    
    FILE *fp, *fp1;
    fp = fopen("ResultadosCAD.txt", "a");

    if (fp == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       exit(0);
    }


    //estruturaCadeia(atoi(argv[1]), fp);
    fclose(fp);

    printf("------------------------------\n");

    fp1 = fopen("ResultadosARV.txt", "a");

    if (fp1 == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       exit(0);
    }

   	estruturaArv(atoi(argv[1]), fp1);
    fclose(fp1);
}
