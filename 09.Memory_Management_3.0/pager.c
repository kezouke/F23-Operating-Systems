#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>

#define MAX_PAGES 1000

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    int counter_for_nfu;
    unsigned char counter_for_aging;
};

int pages;
int frames;
int page_table_size;
struct PTE *pt;
int disk_accesses = 0;
char *RAM[MAX_PAGES];
char *disk[MAX_PAGES];
int free_frames[MAX_PAGES];
char *algorithm;

// ------------------------------------------------------------------------------------------
// code for random()
int my_random(struct PTE* page_table) {
    int random_frame = rand() % frames;
    printf("Chose a random victim page %d for replacement\n", random_frame);
    for (int p = 0; p < pages; p++) {
        if (page_table[p].frame == random_frame) {
            return p;
        }
    }
}

// ------------------------------------------------------------------------------------------
// code for nfu
int nfu(struct PTE* page_table) {
    // Initialize variables for tracking the minimum counter_for_nfu and the page to evict
    int min_counter = INT_MAX;
    int page_to_evict = -1;

    // Iterate through the page table to find the page with the lowest counter_for_nfu
    for (int i = 0; i < pages; i++) {
        if (page_table[i].valid) {
            // Check if the counter_for_nfu of the current page is smaller than the minimum
            if (page_table[i].counter_for_nfu < min_counter) {
                min_counter = page_table[i].counter_for_nfu;
                page_to_evict = i;
            }
        }
    }
    printf("NFU page replacement algorithm chose victim page %d for replacement\n",
           page_to_evict);
    // Return the page number to evict based on the NFU algorithm
    return page_to_evict;
}

// ------------------------------------------------------------------------------------------
// code for aging
int aging(struct PTE* page_table) {
    int victim = -1;
    unsigned char min_counter = 0xFF; // Initialize with the highest value

    for (int i = 0; i < pages; i++) {
        if (page_table[i].valid) {
            if (page_table[i].counter_for_aging < min_counter) {
                min_counter = page_table[i].counter_for_aging;
                victim = i;
                if (min_counter == 0) {
                    // just to optimize
                    break;
                }
            }
        }
    }
    printf("Aging page replacement algorithm chose victim page %d for replacement\n",
           victim);
    return victim;
}
// ------------------------------------------------------------------------------------------

void handle_sigusr1(int signum) {
    // Scan the page table for non-zero referenced values
    int victim = -1;
    for (int i = 0; i < pages; i++) {
        if (pt[i].referenced != 0) {
            victim = i;
            break;
        }
    }

    if (victim != -1) {

        printf("A disk access request from MMU Process (pid=%d)\n",
               pt[victim].referenced);
        int mmu_id = pt[victim].referenced;
        printf("Page %d is referenced\n", victim);

        // Check if there are free frames
        int free_frame_index = -1;
        for (int i = 0; i < frames; i ++) {
            if (free_frames[i] != -1) {
                free_frame_index = i;
                break;
            }
        }
        // printf("free frame index: %d\n", free_frame_index);
        if (free_frame_index != -1) {
            printf("We can allocate it to free frame %d\n", free_frame_index);
            // Allocate a free frame to the page
            pt[victim].valid = true;
            pt[victim].frame = free_frame_index;
            pt[victim].dirty = false;
            pt[victim].referenced = 0;
            free_frames[free_frame_index] = -1;

            printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n",
                   victim, free_frame_index);
            strcpy(RAM[free_frame_index], disk[victim]);

            printf("RAM array\n");
            for(int j = 0; j < frames; j++) {
                printf("Frame %d ---> %s\n", j, RAM[j]);
            }

            disk_accesses++;
            printf("disk accesses is %d so far\n", disk_accesses);

            kill(mmu_id, SIGCONT);
            printf("Resume MMU process\n"
                   "-------------------------\n");
            return;
        }

        printf("We do not have free frames in RAM\n");

        int evict_page;
        if (strcmp(algorithm, "nfu") == 0) {
            evict_page = nfu(pt);
        } else if (strcmp(algorithm, "aging") == 0) {
            evict_page = aging(pt);
        } else if (strcmp(algorithm, "random") == 0) {
            evict_page = my_random(pt);
        }

        printf("Replace/Evict it with page %d to be allocated to frame %d\n",
               victim, pt[evict_page].frame);

        if (pt[evict_page].dirty) {
            // Simulate writing the page to disk
            printf("Victim frame %d is dirty. Writing to disk.\n", pt[evict_page].frame);
            // Copy data from RAM to disk
            // Assuming disk is a global array
            // Assuming RAM is a global array
            // Copy RAM data to disk
            disk_accesses++;
            strcpy(disk[evict_page], RAM[pt[evict_page].frame]);
        }
        printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n",
               victim, pt[evict_page].frame);
        strcpy(RAM[pt[evict_page].frame], disk[victim]);

        // Allocate the frame to the new page
        pt[victim].valid = true;
        pt[victim].frame = pt[evict_page].frame;
        pt[victim].dirty = false;
        pt[victim].referenced = 0;

        // Update page table for victim page
        pt[evict_page].valid = false;
        pt[evict_page].frame = -1;
        pt[evict_page].dirty = false;
        pt[evict_page].referenced = 0;

        printf("RAM array\n");
        for(int j = 0; j < frames; j++) {
            printf("Frame %d ---> %s\n", j, RAM[j]);
        }

        disk_accesses++;
        printf("disk accesses is %d so far\n", disk_accesses);

        kill(mmu_id, SIGCONT);
        printf("Resume MMU process\n"
               "-------------------------\n");
        return;
    } else {
        // No non-zero referenced fields found, terminate the pager
        printf("No pages need loading. Pager terminating. Disk accesses: %d\n", disk_accesses);
        // Unmap the file and delete it
        munmap(pt, sizeof(struct PTE) * pages);
        shm_unlink("/tmp/ex2/pagetable");
        exit(0);
    }
}


char generateRandomChar() {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int charsetSize = sizeof(charset) - 1;

    return charset[rand() % charsetSize];
}

void printBinary(unsigned char value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        char * format_message = "<number of pages> <number of frames> <algorithm>";
        fprintf(stderr, "Usage: %s %s\n", argv[0], format_message);
        exit(1);
    }

    pages = atoi(argv[1]);
    frames = atoi(argv[2]);
    page_table_size = pages * sizeof(struct PTE);
    algorithm = argv[3];


    if (frames > pages) {
        fprintf(stderr, "Number of frames cannot be greater than the number of pages.\n");
        exit(1);
    }

    if (strcmp(algorithm, "random") == 0) {
        printf("Selected Random page replacement algorithm.\n");
    } else if (strcmp(algorithm, "nfu") == 0) {
        printf("Selected NFU page replacement algorithm.\n");
    } else if (strcmp(algorithm, "aging") == 0) {
        printf("Selected Aging page replacement algorithm.\n");
    } else {
        fprintf(stderr, "Unknown algorithm. Use 'random', 'nfu', or 'aging'.\n");
        exit(1);
    }

    // Initialize random seed
    srand(time(NULL));

    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // ftruncate the mapped file
    if (ftruncate(fd, page_table_size) == -1) {
        perror("ftruncate");
        exit(1);
    }

    pt = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (pt == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("-------------------------\n");
    printf("Initialized page table\n");
    for (int i = 0; i < pages; i++) {
        pt[i].valid = false;
        pt[i].frame = -1;
        pt[i].dirty = false;
        pt[i].referenced = 0;
        pt[i].counter_for_aging = 0;
        pt[i].counter_for_nfu = 0;

        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d, "
               "counter_for_nfu=%d, counter_for_aging=",
               i, pt[i].valid, pt[i].frame, pt[i].dirty, pt[i].referenced,
               pt[i].counter_for_nfu);
        printBinary(pt[i].counter_for_aging);
    }
    printf("-------------------------\n\n");


    printf("Initialized RAM\n");
    printf("RAM array\n");
    for (int i = 0; i < frames; i++) {
        RAM[i] = malloc(9); // 8 characters + null terminator
        printf("Frame %d ---> %s\n", i, RAM[i]);
        free_frames[i] = i;
    }
    printf("-------------------------\n\n");


    printf("Initialized disk\n");
    printf("Disk array\n");
    for (int i = 0; i < pages; i++) {
        disk[i] = malloc(9); // 8 characters + null terminator

        // Fill page with random string
        for (int j = 0; j < 8; j++) {
            disk[i][j] = generateRandomChar();
        }
        disk[i][8] = '\0'; // Null-terminate the string

        printf("Page %d ---> %s\n", i, disk[i]);
    }
    printf("-------------------------\n\n");



    // Register the page loaded handler
    signal(SIGUSR1, handle_sigusr1);

    while (1) {
        pause();
    }

    printf("Total Disk Accesses: %d\n", disk_accesses);

    close(fd);
    munmap(pt, page_table_size);

    for (int i = 0; i < pages; i++) {
        free(disk[i]);
    }

    for(int i = 0; i < frames; i++) {
        free(RAM[i]);
    }

    return 0;
}
