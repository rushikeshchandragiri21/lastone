#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_SIZE 101   // A-Z or 1-100 and a null terminator

int main() {
    // Generate a key for the shared memory segment
    key_t key = ftok(".", 'R');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create a shared memory segment
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment
    char *shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Parent process writes data to shared memory
    printf("Enter data (A-Z or 1-100): ");
    fgets(shmaddr, SHM_SIZE, stdin);

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process reads data from shared memory and writes it to a file
        FILE *file = fopen("output.txt", "w");
        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%s", shmaddr);
        fclose(file);

        // Detach the shared memory segment
        if (shmdt(shmaddr) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    } else {
        // Wait for the child process to finish
        wait(NULL);

        // Detach the shared memory segment
        if (shmdt(shmaddr) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

        // Remove the shared memory segment
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
