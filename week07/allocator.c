#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEMORY_SIZE 10000000

// Data structure to represent memory cells
unsigned int memory[MEMORY_SIZE];

// Function to initialize the memory cells to zero
void initializeMemory() {
    memset(memory, 0, sizeof(memory));
}

// Function to find a contiguous free block for allocation using First Fit
int allocateFirstFit(unsigned int adrs, int size) {
    int start = -1;
    int count = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (count == 0) {
                start = i;
            }
            count++;
            if (count == size) {
                for (int j = start; j < start + size; j++) {
                    memory[j] = adrs;
                }
                return start;
            }
        } else {
            count = 0;
        }
    }
    return -1; // Allocation failed
}

// Function to find a contiguous free block for allocation using Best Fit
int allocateBestFit(unsigned int adrs, int size) {
    int start = -1;
    int count = 0;
    int bestStart = -1;
    int bestSize = MEMORY_SIZE;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (count == 0) {
                start = i;
            }
            count++;
        } else {
            if (count >= size) {
                if (count < bestSize) {
                    bestSize = count;
                    bestStart = start;
                }
            }
            count = 0;
        }
    }
    if (count >= size) {
        if (count < bestSize) {
            bestStart = start;
        }
    }
    if (bestStart != -1) {
        for (int j = bestStart; j < bestStart + size; j++) {
            memory[j] = adrs;
        }
        return bestStart;
    }
    return -1; // Allocation failed
}

// Function to find a contiguous free block for allocation using Worst Fit
int allocateWorstFit(unsigned int adrs, int size) {
    int start = -1;
    int count = 0;
    int worstStart = -1;
    int worstSize = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (count == 0) {
                start = i;
            }
            count++;
        } else {
            if (count >= size) {
                if (count > worstSize) {
                    worstSize = count;
                    worstStart = start;
                }
            }
            count = 0;
        }
    }
    if (count >= size) {
        if (count > worstSize) {
            worstStart = start;
        }
    }
    if (worstStart != -1) {
        for (int j = worstStart; j < worstStart + size; j++) {
            memory[j] = adrs;
        }
        return worstStart;
    }
    return -1; // Allocation failed
}

// Function to clear memory associated with a given address
void clear(unsigned int adrs) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == adrs) {
            memory[i] = 0;
        }
    }
}


double getThroughput(int (*algorithm)(unsigned int, int)) {
    FILE *file = fopen("queries.txt", "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    int queryCount = 0;
    clock_t startTime;
    int i = 1;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "end") == 0) {
            break;
        }

        char command[10];
        unsigned int adrs;
        int size;
        if (sscanf(line, "%9s %u %d", command, &adrs, &size) == 3) {
            if (strcmp(command, "allocate") == 0) {
                // Call the appropriate allocation algorithm
                if (i == 1) {
                    startTime = clock();
                    i = 0;
                }
                algorithm(adrs, size);
            } else if (strcmp(command, "clear") == 0) {
                clear(adrs);
            }
            queryCount++;
        }
    }

    clock_t endTime = clock();
    fclose(file);

    // Calculate the throughput and print it
    double totalTime = (double) (endTime - startTime) / CLOCKS_PER_SEC;
    double throughput = queryCount / totalTime;
    return throughput;
}

int main() {
    initializeMemory();

    // Initialize memory cells
    printf("Throughput of FirstFit: %.2f queries per second\n", getThroughput(allocateFirstFit));

    initializeMemory();
    printf("Throughput of BestFit: %.2f queries per second\n", getThroughput(allocateBestFit));

    initializeMemory();
    printf("Throughput of WorstFit: %.2f queries per second\n", getThroughput(allocateWorstFit));

    return 0;
}
