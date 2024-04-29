#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 1024

void takeInput(int *numbers, int n) {
    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &numbers[i]);
    }
}

void display(int *numbers, int n) {
    printf("Shared memory contains:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
}

void bubbleSort(int *numbers, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
}

int main() {
    // Create shared memory segment
    key_t key = ftok("/tmp", 'A');
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    int *numbers = (int *)shmat(shmid, NULL, 0);
    if (numbers == (int *)(-1)) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);
    if (n > SHM_SIZE / sizeof(int)) {
        printf("Number of elements exceeds shared memory size.\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid1, pid2, pid3;
    pid1 = fork();

    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // Child process 1 takes numbers as input
        takeInput(numbers, n);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process waits for child process 1 to complete
        wait(NULL);
        pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid2 == 0) {
            // Child process 2 sorts the numbers
            bubbleSort(numbers, n);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process waits for child process 2 to complete
            wait(NULL);
            pid3 = fork();
            if (pid3 == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid3 == 0) {
                // Child process 3 displays the sorted numbers
                display(numbers, n);
                exit(EXIT_SUCCESS);
            } else {
                // Parent process waits for child process 3 to complete
                wait(NULL);
            }
        }
    }

    // Detach and delete shared memory segment
    shmdt(numbers);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
