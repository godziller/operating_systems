#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

int sched_getcpu(void);

#define MAX 4

int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];

// Thread function to compute one row of matrix multiplication
void* compute_row(void* arg) {
    int row = (int)(long)arg;  // Cast the void* argument back to int
    
    // Get CPU ID for debugging (optional)
    int cpu = sched_getcpu();
    printf("Thread computing row %d running on CPU %d\n", row, cpu);
    
    // Compute the row
    for (int j = 0; j < MAX; j++) {
        matC[row][j] = 0;  // Initialize result cell
        for (int k = 0; k < MAX; k++) {
            matC[row][j] += matA[row][k] * matB[k][j];  // matrix multiplication
        }
    }
    
    return NULL;
}

int main() {
    // Initialize matrices A and B with random values
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }

    // Print matrix A
    printf("Matrix A:\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%d ", matA[i][j]);
        }
        printf("\n");
    }

    // Print matrix B
    printf("\nMatrix B:\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%d ", matB[i][j]);
        }
        printf("\n");
    }

    // Create threads for each row. Only need 1 int..
    pthread_t threads[MAX];
    for (int i = 0; i < MAX; i++) {
        // Pass row number by casting to void*
        pthread_create(&threads[i], NULL, compute_row, (void*)(long)i);
    }

    // Wait for all threads to complete
    for (int i = 0; i < MAX; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix C
    printf("\nResult Matrix C:\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%d ", matC[i][j]);
        }
        printf("\n");
    }

    return 0;
}
