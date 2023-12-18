#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_MESSAGE_SIZE 1024

int main() {
    int pipe_fd[2];
    pid_t publisher_pid, subscriber_pid;

    if (pipe(pipe_fd) == -1) {
        perror("Failed to create pipe");
        exit(EXIT_FAILURE);
    }

    publisher_pid = fork();

    if (publisher_pid == 0) {
        close(pipe_fd[0]);

        char message[MAX_MESSAGE_SIZE];
        printf("Publisher: Enter a message (max %d characters):\n", MAX_MESSAGE_SIZE);
        fgets(message, sizeof(message), stdin);
        write(pipe_fd[1], message, strlen(message));
        close(pipe_fd[1]);

        exit(EXIT_SUCCESS);
    }

    subscriber_pid = fork();
    
    if (subscriber_pid == 0) {
        close(pipe_fd[1]);

        char message[MAX_MESSAGE_SIZE];
        ssize_t bytes_read;

        bytes_read = read(pipe_fd[0], message, sizeof(message));
        if (bytes_read == -1) {
            perror("Failed to read from pipe");
            exit(EXIT_FAILURE);
        }

        printf("Subscriber received: %s", message);
        close(pipe_fd[0]);

        exit(EXIT_SUCCESS);
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}
