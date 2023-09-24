#include <stdio.h>
#include <stdlib.h>


int const_tri(int *p, int n) {
    if (n <= 0) {
        return 0;
    }

    p[0] = 1;
    p[1] = 1;
    p[2] = 2;

    if (n <= 2) {
        return p[n];
    }

    for (int i = 3; i <= n; i++) {
        int temp = p[2];
        p[2] = p[0] + p[1] + p[2];
        p[0] = p[1];
        p[1] = temp;
    }

    return p[2];
}

int main() {
    const int x = 1;
    const int *q = &x;
    int *p = (int *) malloc(sizeof(int) * 3);

    p[0] = *q;
    p[1] = *q;
    p[2] = 2 * *q;

    printf("Memory addresses:\n");
    for (int i = 0; i < 3; i++) {
        printf("p[%d] address: %p\n", i, (void *) &p[i]);
    }

    if (&p[1] - &p[0] == 1 && &p[2] - &p[1] == 1) {
        printf("The cells are contiguous.\n");
    } else {
        printf("The cells are not contiguous.\n");
    }


    printf("Tribonacci number for n=%d: %d\n", 5, const_tri(p, 5));

    free(p);

    return 0;
}
