//shared_memory.c version 22/8
#include "shared_memory.h"

int create_shared_memory(key_t key, size_t size) {
    int shmid = shmget(key, size, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    return shmid;
}

char* attach_shared_memory(int shmid) {
    char* shmaddr = (char*)shmat(shmid, NULL, 0);
    if (shmaddr == (char*)-1) {
        perror("shmat");
        exit(1);
    }
    return shmaddr;
}

void detach_shared_memory(char* shmaddr) {
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }
}

void destroy_shared_memory(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
}
