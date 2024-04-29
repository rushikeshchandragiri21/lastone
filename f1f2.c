#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Semaphore declaration
sem_t sem;

void *f1(void *arg) {
    // Function f1 body
    printf("Enter username: ");
    char username[100];
    scanf("%s", username);
    printf("Username entered: %s\n", username);

    // Post semaphore to allow f2 to execute
    sem_post(&sem);
    
    pthread_exit(NULL);
}

void *f2(void *arg) {
    // Wait for semaphore
    sem_wait(&sem);

    // Function f2 body
    printf("Enter password: ");
    char password[100];
    scanf("%s", password);
    printf("Password entered: %s\n", password);

    pthread_exit(NULL);
}

int main() {
    // Initialize semaphore
    sem_init(&sem, 0, 0);

    // Thread declaration
    pthread_t thread1, thread2;

    // Create threads
    pthread_create(&thread1, NULL, f1, NULL);
    pthread_create(&thread2, NULL, f2, NULL);

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destroy semaphore
    sem_destroy(&sem);

    return 0;
}
