#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

int sched_getcpu(void);

// maximum size of matrix
#define MAX 4

int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];

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
        for (int j = 0; j < MAX; j++)
            printf("%d ", matA[i][j]);
        printf("\n");
    }

    // displaying matB
    printf("Matrix B:\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            printf("%d ", matB[i][j]);
        printf("\n");
    }

    // your code goes here!

    // displaying the result matrix
    printf("Matrix C:\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            printf("%d ", matC[i][j]);
        printf("\n");
    }

    return 0;
}

