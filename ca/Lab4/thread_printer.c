#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

// Function that will be executed by each thread
void* print_cpu_id(void* arg) {
    int thread_id = *((int*)arg);  // Extract thread ID from argument
    printf("Thread %d is running on CPU: %d\n", thread_id, sched_getcpu());
    return NULL;
}

int main() {
    int num_threads;

    // Ask the user for the number of threads
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));  // Dynamically allocate memory for threads
    int* thread_ids = malloc(num_threads * sizeof(int));  // Dynamically allocate memory for thread IDs

    // Create the specified number of threads
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;  // Assign a unique thread ID to each thread
        if (pthread_create(&threads[i], NULL, print_cpu_id, (void*)&thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;  // Exit if thread creation fails
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);  // Wait for each thread to complete
    }

    // Free dynamically allocated memory
    free(threads);
    free(thread_ids);

    return 0;  // Return 0 to indicate successful completion
}

