/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C passando mais de um argumento */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  10 //total de threads a serem criadas

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads;
} t_Args;

//funcao executada pelas threads
void *PrintHello (void *arg) {
  t_Args *args = (t_Args *) arg;

  printf("Sou a thread %d de %d threads\n", args->idThread, args->nThreads);
  free(arg); //aqui pode liberar a alocacao feita na main

  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  t_Args *arg; //receberá os argumentos para a thread

  for(thread=0; thread<NTHREADS; thread++) {
    printf("--Aloca e preenche argumentos para thread %d\n", thread);
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->idThread = thread; 
    arg->nThreads = NTHREADS; 
    
    printf("--Cria a thread %d\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, PrintHello, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  printf("--Thread principal terminou\n");
  pthread_exit(NULL);
}

#include <stdlib.h> 

/****
 *
 * 1) Aqui é passado como um argumento uma estrutura com o nº da thread e nº total de threads
 * 2) As threads são criadas e escalonadas de forma não determinística pelo processador
 * 3) Por isso, a saída pode diferir em cada execução
 * 4) Detalhe para o pthread_exit na main,
 *  que faz com que o processo NÃO seja finalizado antes do término da execução das threads
 *
*/
