#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>

// Define the page table entry structure
struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

// Global variables for MMU
struct PTE *page_table;
int num_pages;
int num_frames;
int pid_pager;

bool paused;

// Function to handle SIGUSR1 signal from pager
void handle_sigcont(int signum) {
    // Check if the page has been loaded
    // Page is now in RAM, continue
    paused = false;
    printf("Received SIGUSR1: MMU resumed by SIGCONT signal from pager\n");
}


int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Not enough arguments!\n"
               "Usage: %s <num_pages> <reference_string> <pid_pager>\n", argv[0]);
        return 1;
    }
    
    signal(SIGCONT, handle_sigcont);

    num_pages = atoi(argv[1]);
    pid_pager = atoi(argv[argc-1]);
    char reference_string[argc-3][10];
    for (int i = 0; i < argc-3; i++) {
        printf("argv: %s\n", argv[i+2]);
        strcpy(reference_string[i], argv[i + 2]);
    }
    num_frames = num_pages; // For simplicity, assume one frame per page

    // Check if the 'pagetable' file exists before opening it
    while (access("/tmp/ex2/pagetable", F_OK) == -1) {
        // Sleep for a while and keep checking
        usleep(100000); // Sleep for 0.1 seconds
    }

    // Open the mapped file
    int fd = open("/tmp/ex2/pagetable", O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    long page_table_size = (long) sizeof(struct PTE) * num_pages;
    // ftruncate the mapped file
    if (ftruncate(fd, page_table_size) == -1) {
        perror("ftruncate");
        exit(1);
    }

    // Map the page table to memory
    page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("-------------------------\n");
    printf("Initialized page table\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("-------------------------\n");

    // Parse the reference string and simulate memory accesses
    for (int i = 0; i < argc-3; i++) {
        printf("-------------------------\n");

        char mode = reference_string[i][0];
        int page = atoi(reference_string[i]+1);
        // printf("mode: %c; page: %d\n", mode, page);
        if (mode == 'W') {
            printf("Write Request for page %d\n", page);
        } else if (mode == 'R') {
            printf("Read Request for page %d\n", page);
        } else {
            printf("mode - %c\n", mode);
        }

        // Check if the page is in RAM
        if (!page_table[page].valid) {
            printf("It is not a valid page --> page fault\n"
                   "Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
            // Page is not in RAM, set referenced and signal pager
            page_table[page].referenced = getpid();
            paused = true;
            kill(pid_pager, SIGUSR1);
            while(paused);
            printf("continiue\n");
        } else {
            printf("It is a valid page\n");
        }

        // Handle write access
        if (mode == 'W') {
            printf("It is a write request then set the dirty field\n");
            page_table[page].dirty = true;
        }

        printf("Page table\n");
        for (int i = 0; i < num_pages; i++) {
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
                   i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
        }
        printf("\n");
    }

    printf("Done all requests.\n"
           "MMU sends SIGUSR1 to the pager.\n"
           "MMU terminates.");

    // Signal the pager for the last time
    kill(pid_pager, SIGUSR1);
    // Unmap the file and close it
    munmap(page_table, sizeof(struct PTE) * num_pages);
    close(fd);

    return 0;
}
