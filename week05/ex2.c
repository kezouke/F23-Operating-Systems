#include <stdio.h>
#include <pthread.h>

struct Thread {
    pthread_t id;
    int i;
    char message[256];
};

void* thread_job(void* arg) {
    struct Thread* thread = (struct Thread*) arg;
    printf("%lu: %s\n", thread->id, thread->message);
    pthread_exit(NULL);
    return NULL;
}


int main() {
    int n;
    printf("type number of threads: ");
    scanf("%d", &n);
    struct Thread threads[n];

    for (int i = 0; i < n; i++) {
        threads[i].i = i;
        sprintf(threads[i].message, "Hello from thread %d", i);
        pthread_create(&threads[i].id, NULL, thread_job, &threads[i]);
        printf("Thread %d is created\n", threads[i].i);
        pthread_join(threads[i].id, NULL);
        printf("Thread %d is finished\n", threads[i].i);
    }
    return 0;
}
