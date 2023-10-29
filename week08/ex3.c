#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    struct rusage usage;
    const int size = 10 * 1024 * 1024; // 10 MB
    char *block;

    for (int i = 0; i < 10; i++) {
        // Allocate memory
        block = (char *)malloc(size);
        if (block == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }

        // Fill memory with zeros
        memset(block, 0, size);

        // Get memory usage
        if (getrusage(RUSAGE_SELF, &usage) == 0) {
            printf("Memory usage: %ld KB\n", usage.ru_maxrss);
            printf("Maximum resident set size (KB): %ld\n", usage.ru_maxrss);
            printf("Minor page faults: %ld\n", usage.ru_minflt);
            printf("Major page faults: %ld\n", usage.ru_majflt);
        } else {
            perror("Failed to get memory usage");
        }

        // Sleep for 1 second
        sleep(1);

    }

    return 0;
}
