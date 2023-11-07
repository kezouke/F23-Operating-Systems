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
#include <time.h>

// Define the page table entry structure
struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

// Define the TLB entry structure
struct TLBEntry {
    int page;
    int frame;
};

// Global variables for MMU
struct PTE *page_table;
struct TLBEntry *tlb;
int num_pages;
int num_frames;
int tlb_size; // Size of the TLB
int pid_pager;

bool paused;
int tlb_hits = 0;
int tlb_misses = 0;

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
    char* reference_string = argv[2];
    pid_pager = atoi(argv[3]);
    num_frames = num_pages; // For simplicity, assume one frame per page
    tlb_size = num_pages * 0.2; // 20% of the page table size for TLB


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

    // Initialize random seed
    srand(time(NULL));

    tlb = (struct TLBEntry *)malloc(sizeof(struct TLBEntry) * tlb_size);


    printf("-------------------------\n");
    printf("Initialized page table\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("-------------------------\n");
    printf("Initialized TLB\n");
    for (int i = 0; i < tlb_size; i++) {
        tlb[i].page = -1;
        tlb[i].frame = -1;
    }
    printf("-------------------------\n");

    // Parse the reference string and simulate memory accesses
    char mode;
    int page;
    for (int i = 0; i < strlen(reference_string); i++) {
        sscanf(&reference_string[i], "%c%d", &mode, &page);

        printf("-------------------------\n");

        // printf("mode: %c; page: %d\n", mode, page);
        if (mode == 'W') {
            printf("Write Request for page %d\n", page);
        } else if (mode == 'R') {
            printf("Read Request for page %d\n", page);
        } else {
            printf("mode - %c\n", mode);
            continue;
        }

        // Check if the page is in the TLB
        bool tlb_hit = false;
        for (int j = 0; j < tlb_size; j++) {
            if (tlb[j].page == page) {
                tlb_hit = true;
                tlb_hits++;
                printf("TLB Hit: Page %d is in TLB, frame=%d\n", page, tlb[j].frame);
                break;
            }
        }

        if (!tlb_hit) {
            tlb_misses++;
            printf("TLB Miss: Page %d is not in TLB\n", page);
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
            // Update TLB with the new mapping
            int tlb_index = -1;
            for (int j = 0; j < tlb_size; j++) {
                if (tlb[j].page == -1) {
                    tlb_index = j;
                    break;
                }
            }
            if (tlb_index == -1) {
                // TLB is full, use a simple replacement policy
                printf("TLB is full, choose random to for replacemnt\n");
                tlb_index = rand() % tlb_size;
            }
            tlb[tlb_index].page = page;
            tlb[tlb_index].frame = page_table[page].frame;
            printf("TLB is updatet\n");
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


    // Print TLB miss ratio
    float tlb_miss_ratio = (float)tlb_misses / (tlb_hits + tlb_misses);
    printf("TLB Miss Ratio: %.2f\n", tlb_miss_ratio);

    printf("Done all requests.\n"
           "MMU sends SIGUSR1 to the pager.\n"
           "MMU terminates.");

    // Signal the pager for the last time
    kill(pid_pager, SIGUSR1);
    // Unmap the file and close it
    munmap(page_table, sizeof(struct PTE) * num_pages);
    free(tlb);
    close(fd);

    return 0;
}
