#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int create_shared_memory(key_t key, size_t size);
char* attach_shared_memory(int shmid);
void detach_shared_memory(char* shmaddr);
void destroy_shared_memory(int shmid);

#endif // SHARED_MEMORY_H
