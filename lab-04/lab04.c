/**
 * Laboratório 4
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
 * ./lab04
 *
 *
 */
#include <pthread.h>
#include <stdio.h>

#define NTHREADS 4

// variáveis globais
int x = 0;
pthread_mutex_t lock;
pthread_cond_t cond_1;
pthread_cond_t cond_2;

void* thread1(void* arg) {
    pthread_mutex_lock(&lock);
    if (x < 3) {
        pthread_cond_wait(&cond_2, &lock);
    }
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

void* thread23(void* arg) {
    const char* msg = arg;

    pthread_mutex_lock(&lock);
    if (x == 0) {
        pthread_cond_wait(&cond_1, &lock);
    }
    ++x;
    printf("%s", msg);
    if (x == 3) { // se já imprimiu "seja", "fique" e "sente-se" sinalize thread
                  // "volte"
        pthread_cond_signal(&cond_2);
    }
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

void* thread4(void* arg) {
    printf("Seja bem-vindo!\n");

    pthread_mutex_lock(&lock);
    ++x;
    pthread_cond_broadcast(&cond_1);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NTHREADS];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond_1, NULL);
    pthread_cond_init(&cond_2, NULL);

    int ret = pthread_create(&threads[0], NULL, thread1, NULL); // Volte sempre!
    ret |= pthread_create(&threads[1], NULL, thread23, "Fique a vontade.\n");
    ret |= pthread_create(&threads[2], NULL, thread23, "Sente-se por favor.\n");
    ret |= pthread_create(&threads[3], NULL, thread4, NULL); // Seja bem-vindo!

    if (ret) {
        fprintf(stderr, "Erro pthread_create");
        return 1;
    }

    pthread_exit(NULL);

    return 0;
}