// The reader-writer problem is a classic synchronization problem in computer science. It deals with multiple processes (or threads) that want to read from and write to a shared resource concurrently. The problem involves ensuring that readers can access the resource simultaneously for reading, while writers have exclusive access to the resource for writing.

// Here's a brief overview of the reader-writer problem:

// - **Readers:** Multiple readers can access the shared resource simultaneously for reading. They do not modify the resource.
// - **Writers:** Writers need exclusive access to the shared resource for writing. While a writer is writing, no other reader or writer should be granted access to the resource.

// The main challenge in implementing the reader-writer problem is to prevent race conditions and ensure the correctness of the shared resource. In many scenarios, allowing multiple readers to access the resource simultaneously is beneficial for performance, but it should not interfere with the exclusive access required by writers to maintain consistency.

// One common solution to the reader-writer problem involves using semaphores or other synchronization mechanisms to coordinate access to the shared resource. Semaphores can be used to enforce the following rules:

// - Readers acquire a read lock before accessing the resource. Multiple readers can acquire the read lock simultaneously.
// - Writers acquire a write lock before accessing the resource for writing. Only one writer can acquire the write lock at a time, and no readers are allowed to access the resource while a writer holds the lock.

// By carefully managing the acquisition and release of locks, it's possible to ensure that readers and writers can access the shared resource safely and efficiently without violating its consistency. However, achieving optimal performance while maintaining correctness requires careful design and implementation.



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2

pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
sem_t mutex, write_mutex;
int readers_count = 0;

void *reader(void *arg) {
    int id = *((int *)arg);
    while (1) {
        sem_wait(&mutex);
        readers_count++;
        if (readers_count == 1) {
            sem_wait(&write_mutex);
        }
        sem_post(&mutex);

        printf("Reader %d is reading...\n", id);
        // Simulating reading operation
        sleep(1);

        sem_wait(&mutex);
        readers_count--;
        if (readers_count == 0) {
            sem_post(&write_mutex);
        }
        sem_post(&mutex);

        // Sleep to simulate intervals between reads
        sleep(1);
    }
    return NULL;
}

void *writer(void *arg) {
    int id = *((int *)arg);
    while (1) {
        sem_wait(&write_mutex);
        
        printf("Writer %d is writing...\n", id);
        // Simulating writing operation
        sleep(2);

        sem_post(&write_mutex);

        // Sleep to simulate intervals between writes
        sleep(2);
    }
    return NULL;
}

int main() {
    int i, ids[NUM_READERS + NUM_WRITERS];
    sem_init(&mutex, 0, 1);
    sem_init(&write_mutex, 0, 1);

    // Create reader threads
    for (i = 0; i < NUM_READERS; i++) {
        ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &ids[i]);
    }

    // Create writer threads
    for (i = 0; i < NUM_WRITERS; i++) {
        ids[i + NUM_READERS] = i + 1;
        pthread_create(&writers[i], NULL, writer, &ids[i + NUM_READERS]);
    }

    // Join reader threads
    for (i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    // Join writer threads
    for (i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&write_mutex);

    return 0;
}
