#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

// Function that will be executed by each thread
// Takes in an int, which represents a unique identifier for each thread
// Then simply prints out that unique id, along with the CPU id its running on.

void* print_cpu_id(void* arg) {
    int thread_id = *((int*)arg);  // Extract thread ID from argument
    printf("Thread %d is running on CPU: %d\n", thread_id, sched_getcpu());
    return NULL;
}

int main() {
    int num_threads;

    // Ask the user for the number of threads the wish to create.
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);	// num_threads int now contains the user input.

    // sizeof(pthread_t) is the pthread.h definition of a thread, and ergo its size.
    // multiplying it by the number of user request threads give is that block of memory - an array.
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));  

    // Also need to dynamically create an array of ints, size dictated by the user input
    int* thread_ids = malloc(num_threads * sizeof(int));  

    // Create the specified number of threads
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;  // Assign a unique thread ID to each thread. Using a simple incremental int.
	
	// First paramater is a pointer to a pthread_t structure, taken from our earlier array.
	// second is this thread attributes - defaulting to NULL here
	// third is the start routing - essentially our function.
	// Fourth is args we pass to the start routine. In our case an int, taken from the int array.
	// Every loop a thread is created and the CPU will land it on the best CPU at the time.	
        if (pthread_create(&threads[i], NULL, print_cpu_id, (void*)&thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;  // Exit if thread creation fails
        }
    }

    // Wait for all threads to finish
    // At this stage all our threads are either running still or finished the simple print.
    // this for loop is essentially going to wait until everything is done - ie all threads done.
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);  // Wait for each thread to complete
    }

    // All that dynamic memory we created earlier, we need to clean it up now.
    free(threads);
    free(thread_ids);

    return 0;  // Return 0 to indicate successful completion
}

