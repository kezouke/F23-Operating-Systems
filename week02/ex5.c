#include <stdio.h>

long long int tribonacci(int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1 || n == 2) {
        return 1;
    }

    long long int t0 = 0, t1 = 1, t2 = 1, tn = 0;

    for (int i = 3; i <= n; i++) {
        tn = t0 + t1 + t2;
        t0 = t1;
        t1 = t2;
        t2 = tn;
    }

    return tn;
}

int main() {
    long long int t4 = tribonacci(4);
    long long int t36 = tribonacci(36);
    printf("tribonacci(4) = %llu\n", t4);
    printf("tribonacci(36) = %llu\n", t36);

    return 0;
}
