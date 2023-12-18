#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int paused = 0;

void read_text() {
    if (!paused) {
    	FILE *file = fopen("text.txt", "r");
    	if (file == NULL) {
       	    perror("Error opening text.txt");
            exit(1);
    	}
    
    	char buffer[1000];
    	while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
    	}
    	fclose(file);
    }
}

void sigstop_handler(int signum) {
    paused = 1;
}

void sigcontinue_handler(int signum) {
    paused = 0;
}

void sigusr1_handler(int signum) {
    read_text();
}

void sigusr2_handler(int signum) {
    printf("Process terminating...\n");
    exit(0);
}

int main() {
    // Create and write PID to /var/run/agent.pid
    FILE *pid_file = fopen("/var/run/agent.pid", "w");
    if (pid_file == NULL) {
        perror("Error creating agent.pid");
        exit(1);
    }
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    signal(SIGSTOP, sigstop_handler);
    signal(SIGCONT, sigcontinue_handler);


    read_text();

    while (1) {
        sleep(1);
    }

    return 0;
}
