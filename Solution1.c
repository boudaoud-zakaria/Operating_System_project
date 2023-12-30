#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// matrix size :
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

    // Initialize matrices B and C
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            B[i][j] = 1;
            C[i][j] = 1;
        }
    }

    // Initialize semaphores
    sem_init(&empty, 0, 1);

    // Create threads
    for (i = 0; i < THREADS; i++) {
        args[i].row = i;
        if(pthread_create(&threads[i], NULL, producer, &args[i]) != 0) {
            perror("pthread_create error !");
        }
    }

    // Wait for the threads to finish
    for (i = 0; i < THREADS; i++) {
        if(pthread_join(threads[i], NULL) != 0){
            perror("pthread_join error!");
        }
    }

    // Display matrix A
    printf("Matrice A :\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }

    // Destroy semaphores
    sem_destroy(&empty);

    return 0;
}
