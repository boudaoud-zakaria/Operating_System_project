#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// matrix size:
#define N1 3
#define M1 4
#define N2 M1  // N2 should be equal to M1 for matrix multiplication
#define M2 5

#define THREADS N1

double B[N1][M1], C[M1][M2], A[N1][M2], TMP[M2];
pthread_t threads[THREADS * 2];
sem_t empty, full;

typedef struct {
    int row;
} thread_args;

void *producer(void *arg) {
    thread_args *args = (thread_args *)arg;
    int j, k;
    double sum;
    sem_wait(&empty);
    for (j = 0; j < M2; j++) {
        sum = 0;
        for (k = 0; k < M1; k++) {
            sum += B[args->row][k] * C[k][j];
        }
        TMP[j] = sum;
    }
    sem_post(&full);
    return NULL;
}

void *consumer(void *arg) {
    thread_args *args = (thread_args *)arg;

    sem_wait(&full);

    for (int j = 0; j < M2; j++) {
        A[args->row][j] = TMP[j];
    }

    sem_post(&empty);

    return NULL;
}

int main() {
    int i, j;
    thread_args args[THREADS];

    // Initialize matrices B and C
    for (i = 0; i < N1; i++) {
        for (j = 0; j < M1; j++) {
            B[i][j] = 1;
        }
    }

    for (i = 0; i < M1; i++) {
        for (j = 0; j < M2; j++) {
            C[i][j] = 1;
        }
    }

    // Initialize semaphores
    sem_init(&empty, 0, 1);  // Initialize empty semaphore to the number of rows
    sem_init(&full, 0, 0);

    // Create threads
    for (i = 0; i < THREADS; i++) {
        args[i].row = i;  // Use modulo to cycle through rows
        if (pthread_create(&threads[i], NULL, producer, &args[i]) != 0) {
            perror("pthread_create error (producer) !");
        }
        if (pthread_create(&threads[i + THREADS], NULL, consumer, &args[i]) != 0) {
            perror("pthread_create error (consumer) !");
        }
    }

    // Wait for the threads to finish
    for (i = 0; i < THREADS * 2; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join error!");
        }
    }

    // Display matrix B
    printf("Matrix B:\n");
    for (i = 0; i < N1; i++) {
        for (j = 0; j < M1; j++) {
            printf("%f ", B[i][j]);
        }
        printf("\n");
    }

    // Display matrix C
    printf("Matrix B:\n");
    for (i = 0; i < N2; i++) {
        for (j = 0; j < M2; j++) {
            printf("%f ", C[i][j]);
        }
        printf("\n");
    }

    // Display matrix A ::: result
    printf("Matrix A:\n");
    for (i = 0; i < N1; i++) {
        for (j = 0; j < M2; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}

