#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define RESOURCES_BOUNDARY 1000
#define THREADS_BOUNDARY 1000

int numResources, numThreads;

pthread_mutex_t resources[RESOURCES_BOUNDARY];
pthread_t *threadIDs;
bool *existingThreads;
bool **RAG;

typedef struct {
    int thread;
    int resource;
} ResourceRequest;

pthread_mutex_t detainedResourceRequestsMutex;
ResourceRequest detainedResourceRequests[THREADS_BOUNDARY];
int detaineesCount = 0;

typedef struct {
    int current;
    int parent;
} Node_RAG;

// Function to check if Resource Allocation Graph (RAG) has cycles using DFS
bool is_RAG_cycled_util(int current, bool *visited, bool **marked) {
    visited[current] = true;

    for (int j = 0; j < numThreads; j++) {
        if (RAG[current][j] && !marked[current][j]) {
            if (!visited[j]) {
                if (is_RAG_cycled_util(j, visited, marked)) {
                    return true;
                }
            } else {
                return true;
            }
        }
    }

    return false;
}

bool is_RAG_cycled() {
    bool *visited = (bool *)calloc(numThreads, sizeof(bool));
    bool **marked = (bool **)calloc(numThreads, sizeof(bool *));
    for (int i = 0; i < numThreads; i++) {
        marked[i] = (bool *)calloc(numThreads, sizeof(bool));
    }

    for (int i = 0; i < numThreads; i++) {
        if (!visited[i]) {
            if (is_RAG_cycled_util(i, visited, marked)) {
                free(visited);
                for (int k = 0; k < numThreads; k++) {
                    free(marked[k]);
                }
                free(marked);
                return true;
            }
        }
    }

    free(visited);
    for (int i = 0; i < numThreads; i++) {
        free(marked[i]);
    }
    free(marked);

    return false;
}

// Function to detain a resource request when a deadlock is detected
void detainResourceRequest(ResourceRequest request) {
    pthread_mutex_lock(&detainedResourceRequestsMutex);
    if (detaineesCount < THREADS_BOUNDARY) {
        detainedResourceRequests[detaineesCount++] = request;
    }
    pthread_mutex_unlock(&detainedResourceRequestsMutex);
}

// Function to print the status of all threads (which mutexes are locked)
void printThreadStatus() {
    printf("Thread Status:\n");
    for (int i = 0; i < numThreads; i++) {
        printf("Thread %d: ", i);
        for (int j = 0; j < numResources; j++) {
            if (RAG[i][j]) {
                printf("Mutex %d is locked | ", j);
            }
        }
        printf("\n");
    }
}

// Function executed for each of the threads:
void *startRoutine(void *arg) {
    ResourceRequest *request = (ResourceRequest *)arg;
    int currentThreadID = request->thread;
    int requestedResourceID = request->resource;

    if (!existingThreads[currentThreadID]) {
        printf("Thread %d is created\n", currentThreadID);
        existingThreads[currentThreadID] = true;
    }

    printf("Thread %d attempts to lock resource mutex %d\n",
           currentThreadID,
           requestedResourceID);

    if (pthread_mutex_trylock(&resources[requestedResourceID]) == 0) {
        printf("Resource mutex %d locked by thread %d\n",
               requestedResourceID,
               currentThreadID);
        RAG[currentThreadID][requestedResourceID] = true;

        if (is_RAG_cycled()) {
            printf("Deadlock detected\n");
            printThreadStatus();

            // Identify and terminate threads involved in the deadlock
            for (int i = 0; i < numThreads; i++) {
                // Check if the current thread holds any resources
                bool hasResources = false;
                for (int j = 0; j < numResources; j++) {
                    if (RAG[i][j]) {
                        hasResources = true;
                        break;
                    }
                }

                // If the thread is involved in the deadlock, terminate it
                if (hasResources) {
                    printf("Trying to resolve deadlock via cancelling thread %d...\n", i);
                    pthread_cancel(threadIDs[i]);
                    existingThreads[i] = false;

                    ResourceRequest pendingRequest;
                    pendingRequest.thread = i;
                    pendingRequest.resource = -1; // Mark as no resource
                    detainResourceRequest(pendingRequest);

                    for (int k = 0; k < numResources; k++) {
                        if (pthread_mutex_trylock(&resources[k]) == 0) {
                            pthread_mutex_unlock(&resources[k]);
                        }
                    }
                }
            }
        } else {
            sleep(rand() % 5);

            pthread_mutex_unlock(&resources[requestedResourceID]);
            printf("Resource mutex %d unlocked by thread %d\n",
                   requestedResourceID,
                   currentThreadID);
            RAG[currentThreadID][requestedResourceID] = false;
        }
    } else {
        printf("Thread %d could not lock resource mutex %d\n",
               currentThreadID,
               requestedResourceID);

        // Detain the resource request
        ResourceRequest pendingRequest;
        pendingRequest.thread = currentThreadID;
        pendingRequest.resource = requestedResourceID;
        detainResourceRequest(pendingRequest);
    }

    free(request);
    return NULL;
}

// Function to read and process resource requests from the input file
void readAndProcessResourceRequests() {
    FILE *file = fopen("input.txt", "r");

    int currentThreadID, requestedResourceID;
    while (fscanf(file, "%d %d", &currentThreadID, &requestedResourceID) == 2) {
        if (currentThreadID < numThreads && requestedResourceID < numResources) {
            ResourceRequest *request = malloc(sizeof(ResourceRequest));
            request->thread = currentThreadID;
            request->resource = requestedResourceID;

            pthread_create(&threadIDs[currentThreadID],
                           NULL,
                           startRoutine,
                           (void *)request);
        } else {
            printf("Invalid thread or resource ID in input file: %d %d\n", currentThreadID, requestedResourceID);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        exit(EXIT_FAILURE);
    }

    numResources = atoi(argv[1]);
    numThreads = atoi(argv[2]);

    if (numResources > RESOURCES_BOUNDARY || numThreads > THREADS_BOUNDARY) {
        fprintf(stderr, "Please, be sure to use no more than 1000 threads and 1000 resources in input.txt!\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    existingThreads = (bool *)calloc(numThreads, sizeof(bool));
    threadIDs = (pthread_t *)malloc(numThreads * sizeof(pthread_t));

    RAG = (bool **)calloc(numThreads, sizeof(bool *));
    for (int i = 0; i < numThreads; i++) {
        RAG[i] = (bool *)calloc(numResources, sizeof(bool));
    }

    for (int i = 0; i < numResources; i++) {
        pthread_mutex_init(&resources[i], NULL);
    }

    pthread_mutex_init(&detainedResourceRequestsMutex, NULL);

    readAndProcessResourceRequests();

    for (int i = 0; i < numThreads; i++) {
        if (existingThreads[i]) {
            pthread_join(threadIDs[i], NULL);
        }
    }

    // Check if no deadlocks occurred
    if (detaineesCount == 0) {
        printf("No deadlocks\n");
    }

    for (int i = 0; i < numResources; i++) {
        pthread_mutex_destroy(&resources[i]);
    }
    pthread_mutex_destroy(&detainedResourceRequestsMutex);

    free(existingThreads);
    free(threadIDs);

    for (int i = 0; i < numThreads; i++) {
        free(RAG[i]);
    }
    free(RAG);

    return 0;
}
