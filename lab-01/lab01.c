/**
 * Laboratório 1
 * Computação concorrente
 *
 * Autor: Diego Costa Ferreira
 *
 * Descrição: Este programa incrementa o valor de cada elemento num vetor em 1
 * utilizando duas threads
 *
 *  A ideia aqui é incrementar as posições pares do vetor
 *  em uma thread e em outra, as ímpares
 *
 */
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 2
#define ARRAY_SIZE 10000

typedef struct {
    int* array;
    bool is_even;
} array_inc_t;

void* increment(void* arg) {
    array_inc_t* arr_inc = (array_inc_t*)arg;
    int* arr = arr_inc->array;

    for (int i = arr_inc->is_even ? 0 : 1; i < ARRAY_SIZE; i += 2) {
        ++arr[i];
    }

    // libera memória
    free(arr_inc);

    pthread_exit(NULL);

    return NULL;
}

int main() {
    pthread_t tid[NTHREADS];

    // cria array e inicializa com zeros
    int* array = calloc(ARRAY_SIZE, sizeof(int));

    for (int i = 0; i < NTHREADS; ++i) {
        array_inc_t* args = malloc(sizeof(array_inc_t));
        if (args == NULL) {
            printf("Erro: malloc\n");
            exit(1);
        }

        args->array = array;
        args->is_even = (i % 2 == 0);

        // retorna zero se deu tudo certo
        if (pthread_create(&tid[i], NULL, increment, (void*)args)) {
            printf("Não foi possível criar thread");
            exit(1);
        }
    }

    // aguarda as threads terminarem
    for (int i = 0; i < NTHREADS; ++i) {
        // retorna zero se tudo certo
        if (pthread_join(tid[i], NULL)) {
            printf("Erro: pthread_join\n");
            exit(1);
        }
    }

    bool is_ok = true;

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        // verifica se o valor foi incrementado corretamente
        if (array[i] != 1) {
            is_ok = false;
            break;
        }
    }

    if (!is_ok) {
        printf("Deu zembra! O vetor não foi incrementado corretamente!\n");
    } else {
        printf("Deu tudo certo. Vetor foi incrementado com sucesso!\n");
    }

    free(array);

    return 0;
}