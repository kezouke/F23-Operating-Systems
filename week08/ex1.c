#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

int main() {
    // Get the process ID (PID)
    pid_t mypid = getpid();

    // Create a file to store PID
    FILE *pidfile = fopen("/tmp/ex1.pid", "w");
    if (pidfile == NULL) {
        perror("Error opening pid file");
        exit(1);
    }
    fprintf(pidfile, "%d\n", mypid);
    fclose(pidfile);

    // Generate a random password that starts with "pass:"
    char password[14];  // 8 characters + null-terminator
    password[0] = 'p';
    password[1] = 'a';
    password[2] = 's';
    password[3] = 's';
    password[4] = ':';

    // Use /dev/urandom to generate the password characters
    FILE *random = fopen("/dev/urandom", "r");
    if (random == NULL) {
        perror("Error opening /dev/urandom");
        exit(1);
    }

    // Generate the next three characters while ensuring they are printable
    for (int i = 5; i < 13; i++) {
        char random_char;
        do {
            fread(&random_char, 1, 1, random);
        } while (!isprint(random_char)); // Check if the character is printable
        password[i] = random_char;
    }

    fclose(random);

    // Make it null-terminated
    password[14] = '\0';

    // Allocate memory for the password
    char *password_copy = (char *)mmap(NULL, sizeof(password), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Copy the password to the shared memory
    strcpy(password_copy, password);
    
    printf("%s\n", password);

    // Wait in an infinite loop
    while (1) {
        sleep(1);
    }

    // Unmapping the memory is typically done on program exit but not needed here.
}

