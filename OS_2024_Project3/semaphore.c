//semaphore.c version 22/8
#include "semaphore.h"

int create_semaphore(key_t key, int initial_value) {
    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }
    initialize_semaphore(semid, 0, initial_value);
    return semid;
}

void initialize_semaphore(int semid, int semnum, int value) {
    if (semctl(semid, semnum, SETVAL, value) < 0) {
        perror("semctl - initialize");
        exit(1);
    }
}

void semaphore_wait(int semid, int semnum) {
    struct sembuf sb;
    sb.sem_num = semnum;
    sb.sem_op = -1;  // P operation
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) == -1) {
        perror("semop P");
        exit(1);
    }
}

void semaphore_signal(int semid, int semnum) {
    struct sembuf sb;
    sb.sem_num = semnum;
    sb.sem_op = 1;  // V operation
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) == -1) {
        perror("semop V");
        exit(1);
    }
}

void destroy_semaphore(int semid) {
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl - destroy");
        exit(1);
    }
}
