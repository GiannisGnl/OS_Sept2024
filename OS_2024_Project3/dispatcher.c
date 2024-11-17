// dispatcher.c version 14/9
#include "shared_memory.h"
#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SM1_KEY 5678
#define SM2_KEY 6789
#define SEM1_KEY 7890
#define SEM2_KEY 8901
#define SEM_CLIENT_READ 8902
#define SHM_SIZE 1024
#define MAX_REQUESTS 1  // Set to 1 (cause there was no definition in description)

int main() {
    int request_count = 0;

    // Set up shared memory and semaphores
    int shmid1 = create_shared_memory(SM1_KEY, SHM_SIZE);
    char *sm1 = (char *)attach_shared_memory(shmid1);

    int shmid2 = create_shared_memory(SM2_KEY, SHM_SIZE);
    char *sm2 = (char *)attach_shared_memory(shmid2);



    int semid1 = create_semaphore(SEM1_KEY, 1);
    initialize_semaphore(semid1, 0, 1);

    int semid2 = create_semaphore(SEM2_KEY, 1);
    initialize_semaphore(semid2, 0, 1);


    //3rd semaphore for better syncing
    int sem_client_read = create_semaphore(SEM_CLIENT_READ, 1);
    initialize_semaphore(sem_client_read, 0, 0);  // Start client in wait state


    printf("Dispatcher running...\n");

    
    // Fork and execute the server process
    pid_t pid = fork();
    if (pid < 0) {
        perror("Failed to fork server process");
        exit(1);
    } else if (pid == 0) {
        // Child process: execute the server
        execl("./server", "./server", "test.txt", NULL);  
        perror("execl failed");  
        exit(1);
    }


    while (request_count < MAX_REQUESTS) {
        // Wait for a client request
        semaphore_wait(semid1, 0);

        // Skip empty requests
        if (strlen(sm1) == 0) continue;



        // Forward request to the server
        strcpy(sm2, sm1);
        memset(sm1, 0, SHM_SIZE);  // Clear sm1 after forwarding

        // Signal the server and wait for a response
        semaphore_signal(semid2, 0);
        semaphore_wait(semid2, 0);

        // Skip empty responses
        if (strlen(sm2) == 0) continue;

        // Forward response to the client
        strcpy(sm1, sm2);
        memset(sm2, 0, SHM_SIZE);  // Clear sm2 after forwarding

        // Signal the client to read the response
        semaphore_signal(sem_client_read, 0);

        request_count++;
    }

    // Signal server to terminate
    strcpy(sm2, "TERMINATE");

    semaphore_signal(semid2, 0);

    printf("Dispatcher handled %d requests, terminating...\n", request_count);



    // Cleanup resources
    detach_shared_memory(sm1);
    detach_shared_memory(sm2);
    destroy_shared_memory(shmid1);
    destroy_shared_memory(shmid2);
    destroy_semaphore(semid1);
    destroy_semaphore(semid2);
    destroy_semaphore(sem_client_read);

    return 0;
}
