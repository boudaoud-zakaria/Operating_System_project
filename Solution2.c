#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// matrix size :
#define N 3
#define THREADS N

double B[N][N], C[N][N], A[N][N], TMP[N];
pthread_t threads[THREADS * 2];
sem_t empty, full;

typedef struct {
    int row;
} thread_args;

void *producer(void *arg) {
    thread_args *args = (thread_args *)arg;
    int j, k, somme;
    sem_wait(&empty);
    for (j = 0; j < N; j++) {
        somme = 0;
        for (k = 0; k < N; k++) {
            somme += B[args->row][k] * C[k][j];
        }
        TMP[j] = somme;
    }
    sem_post(&full);
    return NULL;
}

void *consumer(void *arg) {
    thread_args *args = (thread_args *)arg;

    sem_wait(&full);

    for (int j = 0; j < N; j++) {
        A[args->row][j] = TMP[j];
    }

    sem_post(&empty);

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
    sem_init(&empty, 0, 1);  // Initialize empty semaphore to the number of rows
    sem_init(&full, 0, 0);

    // Create threads
    for (i = 0; i < THREADS; i++) {
        args[i].row = i ;  // Use modulo to cycle through rows
        printf(" the value of (i) :%d \n" , i);
        if(pthread_create(&threads[i], NULL, producer, &args[i]) != 0){
            perror("pthread_create error (producer) !");
        }
        if(pthread_create(&threads[i], NULL, consumer, &args[i]) != 0){
            perror("pthread_create  error (consumer) !");
        }
    }

    // Wait for the threads to finish
    for (i = 0; i < THREADS ; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join error!");
        }
    }

    // Display matrix A
    printf("Matrix A:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
