#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3
#define NUM_VALUES 100

// Structure to pass arguments to the thread function
struct ThreadData {
    double* values;
    int start;
    int end;
};

// Function to generate random numbers
void* generateNumbers(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    for (int i = data->start; i < data->end; i++) {
        data->values[i] = (double)rand() / RAND_MAX * 100; // Generate a random number between 0 and 100
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];
    double values[NUM_THREADS][NUM_VALUES] = {0};
    double sum = 0;

    // Seed the random number generator
    srand(time(NULL));

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].values = values[i];
        threadData[i].start = 0;
        threadData[i].end = NUM_VALUES;
        pthread_create(&threads[i], NULL, generateNumbers, &threadData[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate the average
    for (int i = 0; i < NUM_THREADS; i++) {
        for (int j = 0; j < NUM_VALUES; j++) {
            sum += values[i][j];
        }
    }
    double average = sum / (NUM_THREADS * NUM_VALUES);

    printf("Average: %.2f\n", average);

    return 0;
}
