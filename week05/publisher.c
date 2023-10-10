#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_SUBSCRIBERS 3
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_subscribers>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);

    if (n <= 0 || n > MAX_SUBSCRIBERS) {
        fprintf(stderr,
                "Number of subscribers must be between 1 and %d\n",
                MAX_SUBSCRIBERS);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= n; ++i) {
        char pipe_name[20];
        sprintf(pipe_name, "/tmp/ex1/s%d", i);

        if (mkfifo(pipe_name, 0666) == -1 && errno != EEXIST) {
            perror("Error creating named pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process
    char message[BUFFER_SIZE];

    while (1) {
        printf("Enter message to publish (or type 'exit' to quit): ");
        fgets(message, sizeof(message), stdin);

        for (int i = 1; i <= n; ++i) {
            pid_t pid = fork();

            if (pid == 0) {
                char pipe_name[20];
                sprintf(pipe_name, "/tmp/ex1/s%d", i);

                int fd;
                if ((fd = open(pipe_name, O_WRONLY)) == -1) {
                    perror("Error opening named pipe for writing");
                    exit(EXIT_FAILURE);
                }

           	write(fd, message, strlen(message) + 1);

                close(fd);
                exit(EXIT_SUCCESS);
            }
        }
        
        if (strcmp(message, "exit\n") == 0) {
            break;
        }
        
    }

    return 0;
}
