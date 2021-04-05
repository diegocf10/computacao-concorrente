/**
 * Laboratório 2
 * Computação concorrente
 *
 * Autor: Diego Costa Ferreira
 *
 * Descrição: Este programa faz multiplicação entre matrizes
 * usando n threads
 *
 *
 */
#include "timer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>  // printf
#include <stdlib.h> // atoi malloc

typedef struct matrix_task_t {
    int* first_matrix;
    int* second_matrix;
    int* result_matrix;
    int thread_id;
    int dim;
    int nthreads;
} matrix_task_t;

/***
 *
 * Tarefa para multiplicar matrizes
 *
 * result_matrix = first_matrix * second_matrix
 *
 */
void* mult_matrix_task(void* arg) {
    matrix_task_t* m_t = (matrix_task_t*)arg;
    int* first_matrix = m_t->first_matrix;
    int* second_matrix = m_t->second_matrix;
    int* result_matrix = m_t->result_matrix;
    int nthreads = m_t->nthreads;

    int dim = m_t->dim;
    for (int i = m_t->thread_id; i < dim; i += nthreads) {
        for (int j = 0; j < dim; ++j) {
            int pos = i * dim + j;
            for (int k = 0; k < dim; ++k) {
                result_matrix[pos] +=
                    first_matrix[i * dim + k] * second_matrix[k * dim + j];
            }
        }
    }

    pthread_exit(NULL);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Digite: %s <dimensao_da_matrix_quadrada> "
               "<numero_de_threads>\nExemplo: %s 500 4\n",
               argv[0], argv[0]);
        return 1;
    }

    int dim = atoi(argv[1]);
    int nthreads = atoi(argv[2]);

    double start, end, total = 0.0;

    GET_TIME(start);

    matrix_task_t* args = malloc(sizeof(matrix_task_t) * nthreads);
    pthread_t* tid = malloc(sizeof(pthread_t) * nthreads);
    int* first_matrix = malloc(sizeof(int) * dim * dim);
    int* second_matrix = malloc(sizeof(int) * dim * dim);
    int* result_matrix = malloc(sizeof(int) * dim * dim);

    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            first_matrix[i * dim + j] = 1;
            second_matrix[i * dim + j] = 1;
            result_matrix[i * dim + j] = 0;
        }
    }

    GET_TIME(end);

    printf("Tempo de inicialização: %f\n", end - start);
    total += end - start;

    GET_TIME(start);

    for (int i = 0; i < nthreads; ++i) {
        (args + i)->first_matrix = first_matrix;
        (args + i)->second_matrix = second_matrix;
        (args + i)->result_matrix = result_matrix;
        (args + i)->nthreads = nthreads;
        (args + i)->dim = dim;
        (args + i)->thread_id = i;

        if (pthread_create(tid + i, NULL, mult_matrix_task,
                           (void*)(args + i))) {
            printf("Erro: pthread_create %d\n", i);
            return 3;
        }
    }

    for (int i = 0; i < nthreads; ++i) {
        pthread_join(tid[i], NULL);
    }

    GET_TIME(end);

    printf("Tempo de multiplicação: %f\n", end - start);
    total += end - start;

    bool is_ok = true;

    // verifica a multiplicação
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (result_matrix[i * dim + j] != dim) {
                is_ok = false;
                break;
            }
        }
    }

    if (!is_ok) {
        puts("Deu zebra. Erro na multiplicação");
        return 1;
    }

    GET_TIME(start);

    free(first_matrix);
    free(second_matrix);
    free(result_matrix);
    free(args);
    free(tid);

    GET_TIME(end);

    printf("Tempo de finalização: %f\n", end - start);
    total += end - start;

    printf("Tempo total: %f\n", total);

    return 0;
}