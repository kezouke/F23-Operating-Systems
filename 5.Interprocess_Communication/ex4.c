#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int k = 0;
int c = 0;
int n = 0;

int get_number_to_check() {
    int ret = 0;
    pthread_mutex_lock(&global_lock);
    ret = k;
    if (k < n) {
        k++;
    }
    pthread_mutex_unlock(&global_lock);
    return ret;
}

void increment_primes() {
    pthread_mutex_lock(&global_lock);
    c++;
    pthread_mutex_unlock(&global_lock);
}

void *check_primes(void *arg) {
    while (1) {
        int num = get_number_to_check();
        if (num >= n) {
            break;
        }

        if (is_prime(num)) {
            increment_primes();
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s n m\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];

    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, check_primes, NULL);
    }

    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&global_lock);
    
    printf("n: %d, m: %d, result: %d\n", n, m, c);
    return 0;
}
