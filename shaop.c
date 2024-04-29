#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

void create_memory() {
    key_t key;
    int shmid;

    // Generate a unique key
    if ((key = ftok(".", 'R')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create the shared memory segment
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    printf("Shared memory segment created with ID: %d\n", shmid);
}

void delete_memory() {
    key_t key;
    int shmid;

    // Generate a unique key
    if ((key = ftok(".", 'R')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the shared memory segment ID
    if ((shmid = shmget(key, SHM_SIZE, 0)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Delete the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    printf("Shared memory segment deleted\n");
}

void attach_memory() {
    key_t key;
    int shmid;
    char *shm;

    // Generate a unique key
    if ((key = ftok(".", 'R')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the shared memory segment ID
    if ((shmid = shmget(key, SHM_SIZE, 0)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    printf("Shared memory segment attached at address: %p\n", shm);

    // Detach the shared memory segment
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    printf("Shared memory segment detached\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <operation>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char operation = argv[1][0];

    switch (operation) {
        case 'c':
            create_memory();
            break;
        case 'd':
            delete_memory();
            break;
        case 'a':
            attach_memory();
            break;
        default:
            fprintf(stderr, "Invalid operation: %c\n", operation);
            exit(EXIT_FAILURE);
    }

    return 0;
}
