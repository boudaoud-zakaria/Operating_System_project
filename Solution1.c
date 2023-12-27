#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 3
#define THREADS N

double B[N][N], C[N][N], A[N][N];
pthread_t threads[THREADS];
sem_t empty;

typedef struct {
    int row;
} thread_args;

void *producer(void *arg) {
    thread_args *args = (thread_args *)arg;
    int i, j, k;
    for (j = 0; j < N; j++) {
        sem_wait(&empty);
        A[args->row][j] = 0;
        for (k = 0; k < N; k++) {
            A[args->row][j] += B[args->row][k] * C[k][j];
        }
        sem_post(&empty);
    }
    return NULL;
}

int main() {
    int i, j;
    thread_args args[THREADS];

    // Initialisation des matrices B et C
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            B[i][j] = 1;
            C[i][j] = 1;
        }
    }

    // Initialisation du sémaphore
    sem_init(&empty, 0, 1);

    // Création des threads
    for (i = 0; i < THREADS; i++) {
        args[i].row = i;
        pthread_create(&threads[i], NULL, producer, &args[i]);
    }

    // Attente de la fin des threads
    for (i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Affichage de la matrice A
    printf("Matrice A :\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }

    // Destruction du sémaphore
    sem_destroy(&empty);

    return 0;
}
