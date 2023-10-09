#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <subscriber_index>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int subscriber_index = atoi(argv[1]);

    if (subscriber_index <= 0) {
        fprintf(stderr, "Subscriber index must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    char pipe_name[20];
    sprintf(pipe_name, "/tmp/ex1/s%d", subscriber_index);

    while (access(pipe_name, F_OK) == -1) {
        sleep(1); // Wait for the named pipe to be created
    }


    char buffer[1024];
    while (1) { // Keep reading messages
        int fd = open(pipe_name, O_RDONLY);
	if (fd == -1) {
	    perror("Error opening named pipe for reading");
	    exit(EXIT_FAILURE);
	}
        size_t bytesRead = read(fd, buffer, sizeof(buffer));
        if (bytesRead > 0) {
            if (strcmp(buffer, "exit\n") == 0) {
                close(fd);
                break;  // Exit on receiving "exit" message
            }
            printf("Subscriber %d received: %s\n", subscriber_index, buffer);
        
        }
        close(fd);
    }



    return 0;
}
