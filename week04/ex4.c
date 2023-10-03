#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND_LEN 100
#define MAX_ARG_NUM 10

int main() {
    char command[COMMAND_LEN];
    char *args[MAX_ARG_NUM];

    while (1) {
        printf("ShellByElisei> ");
	fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("Error while scanning command :(\n");
            exit(EXIT_FAILURE);
        }

        
        command[strcspn(command, "\n")] = '\0';

        
        if (strcmp(command, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

       
        char *token = strtok(command, " ");
        int arg_count = 0;
        while (token != NULL && arg_count < MAX_ARG_NUM - 1) {
            args[arg_count] = token;
            token = strtok(NULL, " ");
            arg_count++;
        }
        args[arg_count] = NULL;

        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            exit(0);
        }
    }

    return 0;
}
