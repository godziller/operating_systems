#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

// Function that will be executed by each thread
void* print_cpu_id(void* arg) {
    int thread_id = *((int*)arg); // Extract thread ID from argument
    printf("Thread %d is running on CPU: %d\n", thread_id, sched_getcpu());
    return NULL;
}

int main() {
    pthread_t threads[4];  // Array to store thread IDs
    int thread_ids[4];     // Array to store thread IDs for passing to threads

    // Create 4 threads
    for (int i = 0; i < 4; i++) {
        thread_ids[i] = i;  // Assign a unique thread ID to each thread
        if (pthread_create(&threads[i], NULL, print_cpu_id, (void*)&thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;  // Exit if thread creation fails
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);  // Wait for each thread to complete
    }

    return 0;  // Return 0 to indicate successful completion
}

