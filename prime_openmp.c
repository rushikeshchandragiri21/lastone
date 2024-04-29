#include <stdio.h>
#include <omp.h>

int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int start = 2;
    int end = 100;
    
    printf("Prime numbers between %d and %d:\n", start, end);

    #pragma omp parallel for
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            printf("%d\n", i);
        }
    }

    return 0;
}
