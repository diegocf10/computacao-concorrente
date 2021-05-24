/**
 * Laboratório 7
 * Computação concorrente
 *
 * Autor: Diego Costa Ferreira
 *
 * Descrição: Este programa cria 4 threads, cada qual imprime uma mensagem,
 * respeitando a seguinte ordem: 4, [2, 3] ou [3, 2], 1
 *
 * Thread 1 = "Volte Sempre!"
 * Thread 2 = "Fique a vontade."
 * Thread 3 = "Sente-se por favor."
 * Thread 4 = "Seja bem-vindo!"
 *
 *
 * ./lab07
 *
 *
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define NTHREADS 4

// variáveis globais
int x = 0;
sem_t condt1;
sem_t condt23;

void* thread1(void* arg) {
    sem_wait(&condt1);
    sem_wait(&condt1);

    printf("Volte sempre!\n");

    pthread_exit(NULL);
}

void* thread23(void* arg) {
    const char* msg = arg;

    sem_wait(&condt23);
    printf("%s", msg);
    sem_post(&condt1);

    pthread_exit(NULL);
}

void* thread4(void* arg) {
    printf("Seja bem-vindo!\n");
    sem_post(&condt23);
    sem_post(&condt23);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NTHREADS];

    sem_init(&condt23, 0, 0);
    sem_init(&condt1, 0, 0);

    int ret = pthread_create(&threads[0], NULL, thread1, NULL); // Volte sempre!
    ret |= pthread_create(&threads[1], NULL, thread23, "Fique a vontade.\n");
    ret |= pthread_create(&threads[2], NULL, thread23, "Sente-se por favor.\n");
    ret |= pthread_create(&threads[3], NULL, thread4, NULL); // Seja bem-vindo!

    if (ret) {
        fprintf(stderr, "Erro pthread_create");
        return 1;
    }

    for (int i = 0; i < NTHREADS; ++i) {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Erro pthread_join");
            return 1;
        }
    }

    sem_destroy(&condt23);
    sem_destroy(&condt1);

    return 0;
}