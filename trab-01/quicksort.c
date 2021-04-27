/***
 *
 *
 * Trabalho 1 de Computação Concorrente
 *
 * Autor: Diego Costa Ferreira
 *
 * Descrição: Implementação do algoritmo de ordenação quicksort concorrente
 *
 * ./quicksort <tamanho_do_vetor> [<nivel_de_recursividade>]
 * ./quicksort 100000000
 * ./quicksort 100000000 10
 */

#include "timer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// variável global
int maxLevel = 10;

void swap(int* array, int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int partition(int* array, int lo, int hi) {
    int i = lo;
    int j = hi + 1;

    while (1) {
        /**
         * enquanto for menor que o pivô
         */
        while (array[++i] < array[lo]) {
            if (i == hi)
                break;
        }

        /**
         * enquanto for maior que o pivô
         */
        while (array[lo] < array[--j])
            ;

        // checar se ponteiros se cruzaram
        if (i >= j)
            break;

        swap(array, i, j);
    }

    swap(array, lo, j); // troca j com o pivô

    return j;
}

void qsort_seq(int* array, int lo, int hi) {
    if (hi <= lo)
        return;

    int j = partition(array, lo, hi);

    qsort_seq(array, lo, j - 1);
    qsort_seq(array, j + 1, hi);
}

void quicksort(int* array, int size) { qsort_seq(array, 0, size - 1); }

/**
 *
 * Quicksort concorrente
 */
typedef struct qsort_task_t {
    int* array;
    int level; // nível de recursividade
    int lo;    // [lo, hi]
    int hi;
    int id;
} qsort_task_t;

/***
 *
 * Tarefa para ordernar array usando quicksort
 *
 * */
void* qsort_task(void* arg) {
    qsort_task_t* qstask = (qsort_task_t*)arg;

    int* array = qstask->array;
    int lo = qstask->lo;
    int hi = qstask->hi;
    int level = qstask->level;
    int id = qstask->id;

    // printf("Thread %d [%d, %d] | nível: %d.\n", id, lo, hi, level);

    if (level == maxLevel || hi <= lo) {
        qsort_seq(array, lo, hi);
        // printf("Thread %d finalizada.\n", id);
        pthread_exit(NULL);

        return NULL;
    }

    int j = partition(array, lo, hi);

    qsort_task_t t_args[2];

    t_args[0].lo = lo;
    t_args[0].hi = j - 1;

    t_args[1].lo = j + 1;
    t_args[1].hi = hi;

    pthread_t tid[2];

    for (int i = 0; i < 2; ++i) {
        t_args[i].id = id + i + 1;
        t_args[i].array = array;
        t_args[i].level = level + 1;
        if (pthread_create(tid + i, NULL, qsort_task, t_args + i)) {
            fprintf(stderr, "Erro pthread_create");
            exit(-1);
        }
    }

    for (int i = 0; i < 2; ++i) {
        if (pthread_join(tid[i], NULL)) {
            fprintf(stderr, "Erro pthread_join");
            exit(-1);
        }
    }
    // printf("Thread %d finalizada.\n", id);
    pthread_exit(NULL);
}

void quicksort_conc(int* array, int size) {
    pthread_t tid;
    qsort_task_t t_args;

    t_args.id = 0;
    t_args.lo = 0;
    t_args.hi = size - 1;
    t_args.level = 0;
    t_args.array = array;

    if (pthread_create(&tid, NULL, qsort_task, (void*)(&t_args))) {
        fprintf(stderr, "Erro pthread_create");
        exit(-1);
    }

    if (pthread_join(tid, NULL)) {
        fprintf(stderr, "Erro pthread_join");
        exit(-1);
    }
}

/*
 * Verifica se está ordenado
 */
bool is_sorted(int* array, int size) {
    for (int i = 1; i < size; ++i) {
        if (array[i - 1] > array[i]) {
            return false;
        }
    }

    return true;
}

// gera vetor aleatório
void fill_array_random(int* array, int size) {
    for (int i = 0; i < size; ++i) {
        array[i] = mrand48();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(
            stderr,
            "Digite %s <tamanho_do_vetor> <nivel_recursividade>\nExemplo: %s "
            "100000000 10\n",
            argv[0], argv[0]);
        return 1;
    }

    int num_values = atoi(argv[1]);

    if (argc >= 3) {
        maxLevel = atoi(argv[2]);
    }

    printf("Nível de recursividade: %d\n", maxLevel);

    srand48(time(NULL));

    double start, end;

    int* array = malloc(sizeof(int) * num_values);

    if (array == NULL) {
        fprintf(stderr, "Erro malloc");
        return 2;
    }

    puts("-----Quicksort Sequencial----------");
    printf("Gerando vetor de %d elementos!\n", num_values);

    fill_array_random(array, num_values); // preenche vetor com valores aleatórios

    puts("Ordenando...");

    GET_TIME(start);
    quicksort(array, num_values); // quicksort sequencial
    GET_TIME(end);

    printf("Tempo gasto para ordenar: %f\n", end - start);

    // verifica se vetor está ordenado
    if (is_sorted(array, num_values)) {
        printf("Vetor ordenado com sucesso.\n\n\n");
    } else {
        fprintf(stderr, "Erro ao ordenar vetor\n");
        return 3;
    }

    //---------------------------------------------------

    puts("-----Quicksort Concorrente---------");
    printf("Gerando vetor de %d elementos!\n", num_values);

    fill_array_random(array, num_values); // preenche vetor com valores aleatórios

    puts("Ordenando...");

    GET_TIME(start);
    quicksort_conc(array, num_values); // quicksort concorrente
    GET_TIME(end);

    printf("Tempo gasto para ordenar: %f\n", end - start);

    // verifica se vetor está ordenado
    if (is_sorted(array, num_values)) {
        printf("Vetor ordenado com sucesso.\n\n");
    } else {
        fprintf(stderr, "Erro ao ordenar vetor\n\n");
        return 3;
    }

    free(array);

    return 0;
}