#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int create_semaphore(key_t key, int initial_value);
void initialize_semaphore(int semid, int semnum, int value);
void semaphore_wait(int semid, int semnum);
void semaphore_signal(int semid, int semnum);
void destroy_semaphore(int semid);

#endif // SEMAPHORE_H
