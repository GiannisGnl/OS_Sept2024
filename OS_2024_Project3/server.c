// server.c, me Termination version 31/8

#include "shared_memory.h"
#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SM2_KEY 6789
#define SEM2_KEY 8901
#define SHM_SIZE 1024

// Func to grab a specific line from a text file
void get_line_from_file(const char *filename, int line_number, char *buffer, size_t buffer_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file!"); 
        exit(1);
    }

    int current_line = 0;
    while (fgets(buffer, buffer_size, file)) {
        if (++current_line == line_number) {
            //found the line, strip newline and break
            buffer[strcspn(buffer, "\n")] = '\0';
            break;
        }
    }

    if (current_line < line_number) {
        strcpy(buffer, "Line not found"); // Handle case where line is missing
    }

    fclose(file); // Close file after reading
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]); // correct way to use this prog
        exit(1);
    }

    const char *filename = argv[1];  // Filename from command line args

    // Create shared memory for SM2 (Dispacher to Server)
    int shmid2 = create_shared_memory(SM2_KEY, SHM_SIZE);
    char *sm2 = (char *)attach_shared_memory(shmid2);

    // Initialize shared memoryy
    memset(sm2, 0, SHM_SIZE);

    // Create semaphore for sync
    int semid2 = create_semaphore(SEM2_KEY, 1);
    initialize_semaphore(semid2, 0, 1);

    //Beauty print for start
    printf("Server is runnin...\n");

    while (1) {
        // Wait for request from dispatcher 
        semaphore_wait(semid2, 0);

        // Check for termination signal
        if (strcmp(sm2, "TERMINATE") == 0) {
            printf("Server got the termination signal. Exiting now...\n");
            break;
        }

        // Read the request from shared mem (line num)
        int line_number = atoi(sm2);
        //printf("Server got request for line num: %d\n", line_number); // debug print

        // If line num is 0 or negative, skip
        if (line_number <= 0) {
            //printf("Invalid line num, gonna skip it...\n"); // debug print
            continue;
        }

        // Get the specific line from file
        char response[SHM_SIZE];
        get_line_from_file(filename, line_number, response, SHM_SIZE);

        //printf("Server got the line: %s\n", response); // Debug print

        // Put the response back to the dispatcher thru shared mem
        strcpy(sm2, response);
        //printf("Server sent response: %s\n", sm2); // Debug print

        // Signal the dispatcher
        //printf("Server is signaling dispatcher...\n"); // Debug print
        semaphore_signal(semid2, 0);
    }

    // Cleanup, Detach only not destroy here
    detach_shared_memory(sm2);

    return 0;
}
