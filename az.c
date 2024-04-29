#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 26  // Size of shared memory segment for characters ('A' to 'Z')

int main() {
    int shmid;
    key_t key = ftok(".", 'A'); // Generate a key for the shared memory segment

    // Create the shared memory segment
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    char *shm = shmat(shmid, NULL, 0);
    if (shm == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Sending 'A' to 'Z' to shared memory...\n");

    // Write data to shared memory ('A' to 'Z')
    for (int i = 0; i < SHM_SIZE; i++) {
        shm[i] = 'A' + i;
    }

    printf("Data sent to shared memory successfully.\n");

    // Detach the shared memory segment
    shmdt(shm);

    // Create another process to receive and print the data from the shared memory
    if (fork() == 0) {
        // Child process
        
        // Reattach the shared memory segment
        char *shm_child = shmat(shmid, NULL, 0);
        if (shm_child == (char *) -1) {
            perror("shmat");
            exit(1);
        }

        printf("Received data: ");
        
        // Print the received data from shared memory
        for (int i = 0; i < SHM_SIZE; i++) {
            printf("%c ", shm_child[i]);
        }
        printf("\n");

        // Detach the shared memory segment
        shmdt(shm_child);

        exit(0);
    } else {
        // Parent process
        wait(NULL); // Wait for the child process to complete
    }

    // Clean up: Delete the shared memory segment
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
