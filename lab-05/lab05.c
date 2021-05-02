/**
 * Laboratório 5
 * Computação concorrente
 *
 * Autor: Diego Costa Ferreira
 *
 * Descrição: Implementa a solução para o problema de soma de prefixo
 *
 * ./lab05 <tamanho_do_array_potencia_de_2>
 * ./lab05 4
 *  Cria array 2⁴ elementos
 */
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// variáveis globais
int blocked = 0;
pthread_mutex_t lock;
pthread_cond_t cond;

void barrier(int nthreads) {
    pthread_mutex_lock(&lock);
    if (blocked == (nthreads - 1)) {
        pthread_cond_broadcast(&cond);
        blocked = 0;
    } else {
        blocked++;
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);
}

typedef struct task_t {
    int* array;
    int id;
    int size;
} task_t;

/**
 *
 * Tarefa
 */
void* task(void* arg) {
    task_t* t = (task_t*)(arg);
    int id = t->id;
    int size = t->size;
    int* array = t->array;
    for (int i = 1; i <= size; i *= 2) {
        int val;
        int pos = id - i;
        if (pos >= 0) {
            val = array[pos];
        }
        barrier(size);
        if (pos >= 0) {
            array[id] += val;
        }
        barrier(size);
    }

    pthread_exit(NULL);
}

// verifica soma
// returna true se tudo OK
bool check_sum(int* array, int size) {
    int lastsum = 0;
    for (int i = 0; i < size; ++i) {
        lastsum += (i + 1);
        if (array[i] != lastsum) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr,
                "Digite %s <potência_de_2_para_tamanho_do array>.\nExemplo: %s "
                "4\nCria um array 2⁴ (16) elementos.\n",
                argv[0], argv[0]);
        return 1;
    }

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    int n = atoi(argv[1]);
    int size = pow(2, n);
    int* array = malloc(sizeof(int) * size);
    pthread_t* tid;
    task_t* targs;

    // preenche array [1, 2, 3...]
    for (int i = 0; i < size; ++i) {
        array[i] = i + 1;
    }

    tid = malloc(sizeof(pthread_t) * size);
    targs = malloc(sizeof(task_t) * size);

    puts("Iniciando threads...");

    for (int i = 0; i < size; ++i) {
        targs[i].id = i;
        targs[i].size = size;
        targs[i].array = array;
        if (pthread_create(tid + i, NULL, task, targs + i)) {
            fprintf(stderr, "Erro pthread_create!\n");
            return 2;
        }
    }

    for (int i = 0; i < size; ++i) {
        if (pthread_join(tid[i], NULL)) {
            fprintf(stderr, "Erro pthread_join!\n");
        }
    }

    puts("Soma finalizada.\nVerificando soma...");

    // verifica soma
    if (check_sum(array, size)) {
        puts("Soma efetuada corretamente.");
    } else {
        puts("Deu zebra! Erro na soma.");
    }

    free(array);
    free(tid);
    free(targs);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}