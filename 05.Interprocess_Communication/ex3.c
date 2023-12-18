#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>

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

//Primes counter in [a, b)
int primes_count(int a, int b) {
    int ret = 0;
    for (int i = a; i < b; i++) {
        if (is_prime(i) == true) {
            ret++;
        }
    }
    return ret;
}

//argument to the start_routine of the thread
typedef struct prime_request {
    int a, b;
} prime_request;

//start_routine of the thread
void *prime_counter(void *arg) {
    //get the request from arg
    prime_request *req = (prime_request *) arg;

    //perform the request
    int *count = (int*) malloc(sizeof(int));
    int result = primes_count(req->a, req->b);
    *count = result;

    return ((void *) count);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s n m\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];
    prime_request requests[m];
    int total_primes = 0;

    int step = n / m;
    for (int i = 0; i < m; i++) {
        requests[i].a = i * step;
        requests[i].b = (i == m - 1) ? n : (i + 1) * step;
    }

    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, prime_counter, &requests[i]);
    }

    for (int i = 0; i < m; i++) {
        void *result;
        pthread_join(threads[i], &result);
        total_primes += *((int *)result);
        free(result);
    }

    printf("n: %d, m: %d, result: %d\n", n, m, total_primes);

    return 0;
}
