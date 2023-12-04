#include <stdio.h>
#include <stdlib.h>

#define PROCESSES_BOUNDARY 10
#define RESOURCES_BOUNDARY 10

int available_resources[RESOURCES_BOUNDARY];
int current_allocation[PROCESSES_BOUNDARY][RESOURCES_BOUNDARY];
int resource_requests[PROCESSES_BOUNDARY][RESOURCES_BOUNDARY];
int work_vector[RESOURCES_BOUNDARY];
int process_finish[PROCESSES_BOUNDARY];

void input_data(const char *file_name, int *n_p, int *n_r) {
    FILE *file = fopen(file_name, "r");

    // Read the number of resources and processes from the file
    fscanf(file, "%d %d", n_r, n_p);

    // Read the available resources and initialize the work vector
    for (int i = 0; i < *n_r; i++) {
        fscanf(file, "%d", &available_resources[i]);
        work_vector[i] = available_resources[i]; // Initialize work vector
    }

    // Read the current allocation matrix
    for (int i = 0; i < *n_p; i++) {
        for (int j = 0; j < *n_r; j++) {
            fscanf(file, "%d", &current_allocation[i][j]);
        }
    }

    // Read the resource request matrix
    for (int i = 0; i < *n_p; i++) {
        for (int j = 0; j < *n_r; j++) {
            fscanf(file, "%d", &resource_requests[i][j]);
        }
    }

    fclose(file);
}

// Check if a specific prcs's request can be fulfilled
int canFulfillRequest(int prcs, int n_r) {
    for (int j = 0; j < n_r; j++) {
        // If the request exceeds the available resources, it cannot be fulfilled
        if (resource_requests[prcs][j] > work_vector[j]) {
            return 0;
        }
    }
    return 1;
}

// the Banker's Algorithm to detect deadlocks
int bankersAlgorithm(int n_p, int n_r) {
    // Initialize work vector and process finish vector
    for (int i = 0; i < n_r; i++) {
        work_vector[i] = available_resources[i];
    }
    for (int i = 0; i < n_p; i++) {
        process_finish[i] = 0;
    }

    int is_found;
    do {
        is_found = 0;
        for (int i = 0; i < n_p; i++) {
            if (!process_finish[i] && canFulfillRequest(i, n_r)) {
                // Mark the process as finished
                process_finish[i] = 1;

                // Release resources of the finished process
                for (int k = 0; k < n_r; k++) {
                    work_vector[k] += current_allocation[i][k];
                }

                is_found = 1; // Set the flag to indicate progress
            }
        }
    } while (is_found); // Continue until no more progress is made

    for (int i = 0; i < n_p; i++) {
        // If a process is not finished, it is deadlocked
        if (!process_finish[i]) {
            printf("Deadlock occurred! Processes involved: ");
            printf("%d\n", i); // Print the deadlocked process
            return 0;
        }
    }

    return 1;

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n_p, n_r;
    input_data(argv[1], &n_p, &n_r);

    int deadlock = bankersAlgorithm(n_p, n_r);

    if (deadlock) {
        printf("No deadlock detected.\n");
    }

    return EXIT_SUCCESS;
}
