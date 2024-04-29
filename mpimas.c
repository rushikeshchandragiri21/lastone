#include <stdio.h>
#include <omp.h>

int main() {
    // Number of threads to use
    int num_threads = 4;

    // Set the number of threads
    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();

        // Master thread counts the total number of threads
        #pragma omp master
        {
            printf("Total number of threads created: %d\n", total_threads);
        }

        // Other threads print their thread numbers
        printf("Thread %d: Hello from thread %d of %d\n", thread_id, thread_id, total_threads);
    }

    return 0;
}
