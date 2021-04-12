/**
 * Laboratório 3
 * Computação concorrente
 *
 * Autor: Diego Costa Ferreira
 *
 * Descrição: Este programa calcula pi usando Fórmula de Leibniz
 *
 * ./lab03 <número_de_termos> <num_de_threads>
 * ./lab03 1000000000 4
 *
 *
 * pi = 4 * Somatório(i = 0, i = n)  (-1)^n/(2*i + 1)
 *
 **/

#include "timer.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Calcula pi sequencialmente
 **/
double calculate_pi(long long int N) {
    double sum = 0.0;
    double alt = 1.0;
    for (long long int i = 0; i < N; ++i) {
        sum += alt / (2 * i + 1);
        alt = -alt;
    }

    return 4 * sum;
}

typedef struct leibniz_task_t {
    int tid;
    int nthreads;
    long long int num_terms;
} leibniz_task_t;

/**
 *
 * Tarefa para calcular concorrentemente
 *  os termos da série leibniz para pi
 *
 **/

void* calculate_sum_pi_task(void* arg) {
    leibniz_task_t* m_t = (leibniz_task_t*)arg;

    double* sum = malloc(sizeof(double));
    if (sum == NULL) {
        fprintf(stderr, "ERRO: malloc double");
        exit(1);
    }
    *sum = 0.0;

    int tid = m_t->tid;
    int nthreads = m_t->nthreads;

    long long int block_size = m_t->num_terms / nthreads;
    long long int start = tid * block_size;
    long long int end;

    if (tid == nthreads - 1) {
        end = m_t->num_terms;
    } else {
        end = start + block_size;
    }

    double alt = pow(-1.0, start);
    for (long long int i = start; i < end; ++i) {
        *sum += alt / (2 * i + 1);
        alt = -alt;
    }

    pthread_exit((void*)sum);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr,
                "Digite %s <número_de_termos> <número_de_threads>\nExemplo: %s "
                "1000000000 4\n",
                argv[0], argv[0]);
        return 1;
    }
    double start, end, elapsed, total = 0.0;
    long long int num_of_terms = atoll(argv[1]);

    /***
     *
     *  Calcula pi de forma sequencial
     *
     */
    puts("### Calcula PI de forma sequencial ###\n");

    GET_TIME(start);
    double pi = calculate_pi(num_of_terms);
    GET_TIME(end);

    printf("PI usando leibniz: %.15f\n", pi);
    printf("PI da math.h: %.15f\n", M_PI);
    printf("Épsilon (PI leibiz - PI math.h): %.15f\n", fabs(pi - M_PI));
    printf("Tempo total: %f\n", end - start);

    puts("\n============#================\n");

    /***
     *
     *  Calcula pi de forma concorrente
     *
     */

    puts("### Calcula PI de forma concorrente: ###\n");

    int nthreads = atoi(argv[2]);

    GET_TIME(start);

    leibniz_task_t* args =
        (leibniz_task_t*)malloc(sizeof(leibniz_task_t) * nthreads);

    if (args == NULL) {
        fprintf(stderr, "Erro malloc leibniz_task_t");
        return 1;
    }

    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t) * nthreads);

    if (tid == NULL) {
        fprintf(stderr, "Erro malloc pthread_t");
        return 1;
    }

    GET_TIME(end);

    elapsed = end - start;

    printf("Tempo de inicialização: %f\n", elapsed);

    total += elapsed;

    GET_TIME(start)

    for (int i = 0; i < nthreads; ++i) {
        (args + i)->num_terms = num_of_terms;
        (args + i)->nthreads = nthreads;
        (args + i)->tid = i;
        if (pthread_create(tid + i, NULL, calculate_sum_pi_task,
                           (void*)(args + i))) {
            fprintf(stderr, "ERRO: pthread_create %d", i);
            return 1;
        }
    }

    double sum = 0.0;
    double* cur;
    for (int i = 0; i < nthreads; ++i) {
        if (pthread_join(tid[i], (void**)&cur)) {
            fprintf(stderr, "ERRO: pthread_join %d", i);
            return 1;
        }
        sum += *cur;
        free(cur);
    }

    GET_TIME(end);

    pi = 4.0 * sum;

    printf("PI usando leibniz: %.15f\n", pi);
    printf("PI da math.h: %.15f\n", M_PI);
    printf("Épsilon: %.15f\n", fabs(pi - M_PI));

    elapsed = end - start;

    printf("Tempo de processamento: %f\n", elapsed);

    total += elapsed;

    GET_TIME(start);

    free(args);
    free(tid);

    GET_TIME(end);

    elapsed = end - start;

    printf("Tempo de finalização: %f\n", elapsed);

    total += elapsed;

    printf("Tempo total: %f\n", total);

    return 0;
}