#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>

#define FILE_SIZE (500 * 1024 * 1024)  // 500 MiB

int main() {
    // Step 1: Create an empty file 'text.txt'
    int fd = open("text.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Failed to create text.txt");
        return 1;
    }

    long page_size = sysconf(_SC_PAGESIZE);
    long chunk_size = 1024 * page_size;

    // Step 2: Generate content from /dev/random and write to text.txt
    FILE *random = fopen("/dev/random", "r");
    FILE *text = fdopen(fd, "w");

    int character;
    // Open the text_file for reading
    unsigned long long currentSize = 0;
    // Read characters one by one until EOF is reached
    int counter = 0;
    while (currentSize < FILE_SIZE) {
        character = fgetc(random);
        if(isprint(character)){
            fputc(character, text);
            currentSize++;
            counter++;
            if(counter==1024){
                fputc('\n', text);
                counter=0;
            }
        }

    }

    // Close the text_file
    fclose(text);
    fclose(random);


    fd = open("text.txt", O_RDWR);
    if (fd == -1) {
        perror("Failed to open text.txt for reading");
        return 1;
    }

    // Step 4: Memory map the file
    off_t file_size = lseek(fd, 0, SEEK_END);
    // printf("file_size: %lu\n", file_size);
    lseek(fd, 0, SEEK_SET);

    long count = 0;
    for (off_t offset = 0; offset < file_size; offset += chunk_size) {
        long chunk_start = offset;
        long chunk_end = offset + chunk_size;
        if (chunk_end > file_size) {
            chunk_end = file_size;
        }

        long chunk_length = chunk_end - chunk_start;

        char *data = mmap(NULL, chunk_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, chunk_start);
        if (data == MAP_FAILED) {
            perror("mmap failed");
            return 1;
        }

        // Step 5: Count capital letters in the mapped chunk
        int capital_count = 0;
        for (int i = 0; i < chunk_length; i++) {
            if (isupper(data[i])) {
                capital_count++;
            }
        }
        // printf("Chunk from offset %ld to %ld has %d capital letters.\n", chunk_start, chunk_end, capital_count);
        count += capital_count;
        // Step 6: Replace capital letters with lowercase
        for (int i = 0; i < chunk_length; i++) {
            if (isupper(data[i])) {
                data[i] = tolower(data[i]);
            }
        }

        // Step 7: Unmap the memory
        munmap(data, chunk_length);
    }
    printf("Overall capital letters: %lu\n", count);

    // Close the file
    close(fd);

    return 0;
}
