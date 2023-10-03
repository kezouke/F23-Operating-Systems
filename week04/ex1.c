#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    clock_t start = clock();
    
    pid_t first_fork = fork();
    
    clock_t child1_start = clock();
    
    if (first_fork == 0) {
        printf("Child process 1; PID: %d, PPID: %d, Execution time: %f ms\n", 
        getpid(), getppid(), ((float)(clock() - child1_start)*1000) / CLOCKS_PER_SEC);
        exit(0);
    } else {
    	pid_t second_fork = fork();
    	clock_t child2_start = clock();
        if (second_fork == 0) {
            printf("Child process 2: PID: %d, PPID: %d, Execution time: %f ms\n",
            getpid(), getppid(), ((float)(clock() - child2_start)*1000) / CLOCKS_PER_SEC);
            exit(0);
        } else {
            printf("Parent Process: PID: %d, PPID: %d, Execution time: %f ms\n",
            getpid(), getppid(), ((float)(clock() - start)*1000) / CLOCKS_PER_SEC);
        }
        wait(NULL);
    	wait(NULL);
    	return 0;
    }
}
