#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define INCREMENTS 100000

int counter = 0; // Shared global counter - this is what we need to protect from race-condition access.

void *increment_counter(void *arg) {
    for (int i = 0; i < INCREMENTS; i++) {
        counter++;
    }
    return NULL;
}

int main() {

    // In lab 4 we had to use malloc to dynamically create this.
    // here however as we know the number of threads we can statically create the array
    // of threads.
    pthread_t threads[NUM_THREADS];

    // The interesting thing here is the final NULL.
    // As our thread function takes no arguements, we can just use NULL here.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_counter, NULL);
    }
    

    // All threads done - we wait until everyone is complete.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d (Expected: %d)\n", counter, NUM_THREADS * INCREMENTS);

    return 0;
}
