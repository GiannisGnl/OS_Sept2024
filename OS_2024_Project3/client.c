// client.c version 31/8
#include "shared_memory.h"
#include "semaphore.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define SM1_KEY 5678

#define SEM1_KEY 7890
#define SEM_CLIENT_READ 8902
#define SHM_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <line_number>\n", argv[0]);
        exit(1);
    }

    int line_number = atoi(argv[1]);

    // Set up shared memory and semaphores
    int shmid1 = create_shared_memory(SM1_KEY, SHM_SIZE);
    char *sm1 = (char *)attach_shared_memory(shmid1);

    int semid1 = create_semaphore(SEM1_KEY, 1);
    initialize_semaphore(semid1, 0, 1);

    int sem_client_read = create_semaphore(SEM_CLIENT_READ, 1);
    initialize_semaphore(sem_client_read, 0, 0);  // Initialize to 0 for client waiting

    // Measure the start time
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);



    // Clear shared memory and send the request
    memset(sm1, 0, SHM_SIZE);
    snprintf(sm1, SHM_SIZE, "%d", line_number);
    printf("Requesting line number: %d\n", line_number);

    // Signal the dispatcher to process the request
    semaphore_signal(semid1, 0);

    // Wait for the response from the dispatcher
    semaphore_wait(sem_client_read, 0);



    // Read the response from shared memory
    if (strlen(sm1) > 0) {
        printf("Received response: %s\n", sm1);
        memset(sm1, 0, SHM_SIZE);  // Clear memory after reading
    } else {
        printf("No response received or response is empty.\n");
    }

    // Measure and print the elapsed time
    gettimeofday(&end_time, NULL);
    double elapsed = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) * 1e-6;
    printf("Request handled in %.6f seconds.\n", elapsed);

    // Cleanup, Detach not destroy
    detach_shared_memory(sm1);

    return 0;
}
