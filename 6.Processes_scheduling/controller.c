#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

pid_t agent_pid = 0;

void sigint_handler(int signum) {
    if (agent_pid > 0) {
        kill(agent_pid, SIGTERM);
    }
    exit(0);
}

int main() {
    FILE *pid_file = fopen("/var/run/agent.pid", "r");
    if (pid_file == NULL) {
        printf("Error: No agent found.\n");
        exit(1);
    }
    
    signal(SIGTERM, sigint_handler);

    fscanf(pid_file, "%d", &agent_pid);
    fclose(pid_file);

    printf("Agent found.\n");

    while (1) {
        printf("Choose a command {“read”, “exit”, “stop”, “continue”}: ");
        char command[20];
        scanf("%s", command);

        if (strcmp(command, "read") == 0) {
            kill(agent_pid, SIGUSR1);
        } else if (strcmp(command, "exit") == 0) {
            kill(agent_pid, SIGUSR2);
            exit(0);
        } else if (strcmp(command, "stop") == 0) {
            kill(agent_pid, SIGSTOP);
        } else if (strcmp(command, "continue") == 0) {
            kill(agent_pid, SIGCONT);
        }
    }

    return 0;
}
