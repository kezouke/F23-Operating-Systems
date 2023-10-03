#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments!\n");
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Please provide a positive integer value for n.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        int pid = fork();
	sleep(5);
	
    }
    sleep(30);
    return 0;
}
