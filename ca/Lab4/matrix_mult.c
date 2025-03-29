#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#define MAX 4

// Note these are global arrays accessable by all threads
// but our code only scopes to unique rows so no thread
// will step on another threads toes when storing to results 
// matrix.
int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];

// Thread function to compute one row of matrix multiplication
// the arg parameter is the row identifier
// Each thread should only focus on 1 row.
void* compute_row(void* arg) {
    // we need to cast it back into an int.
    int row = (int)(long)arg;  // Cast the void* argument back to int
    
    // Get CPU ID to show how threads are scheduled on different CPUs.
    int cpu = sched_getcpu();
    printf("Thread computing row %d running on CPU %d\n", row, cpu);
    
    // Compute the row - a simple nested loop focusing on 1 row and column
    // and saving the result to the results matrix matC[row]
    // we can safely do this because each thread is only accessing their
    // scoped view of the results matrix.
    for (int j = 0; j < MAX; j++) {
        matC[row][j] = 0;  // Initialize result cell
        for (int k = 0; k < MAX; k++) {
            matC[row][j] += matA[row][k] * matB[k][j];  // matrix multiplication
        }
    }
    
    return NULL;
}

int main() {
    // generating random values in matA and matB
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }

    // displaying matA
    printf("Matrix A:\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%d ", matA[i][j]);
        }
        printf("\n");
    }

    // displaying matB
    printf("\nMatrix B:\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%d ", matB[i][j]);
        }
        printf("\n");
    }

    // Create threads for each row. Only need 1 int..
    pthread_t threads[MAX]; // an array of threads.
    for (int i = 0; i < MAX; i++) {
        // Pass row number by casting to void*
	// First paramater is a pointer to a pthread_t structure, taken from our earlier array.
	// second is this thread attributes - defaulting to NULL here
	// third is the start routing - essentially our function.
	// Fourth is args we pass to the start routine. In our case an int, that increments each loop.
	// Every loop a thread is created and the CPU will land it on the best CPU at the time.
        pthread_create(&threads[i], NULL, compute_row, (void*)(long)i);
    }

    // Wait for all threads to complete
    for (int i = 0; i < MAX; i++) {
        pthread_join(threads[i], NULL);
    }

    // displaying the result matrix
    printf("\nResult Matrix C:\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%d ", matC[i][j]);
        }
        printf("\n");
    }

    return 0;
}

