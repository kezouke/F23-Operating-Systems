#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <time.h>

// Declare a global variable to store the path
char *path;

// Function to print detailed stat information for a given file or directory
void print_stat_info_for_file(const char *absolute_path_to_the_entry) {
    // Declare an array to store the full path
    char full_path[PATH_MAX];
    // Construct the full path
    sprintf(full_path, "%s/%s", path, absolute_path_to_the_entry);

    // Declare a structure to store file status
    struct stat st;
    if (stat(full_path, &st) == 0) {
        printf("Stat info for %s:\n", full_path);
        printf("  Size: %ld bytes\n", st.st_size);
        printf("  Inode: %ld\n", st.st_ino);
        printf("  Number of Hard Links: %ld\n", st.st_nlink);
        printf("  Last Access Time: %s", ctime(&st.st_atime));
        printf("  Last Modification Time: %s", ctime(&st.st_mtime));
        printf("  Last Status Change Time: %s", ctime(&st.st_ctime));
    }
    printf("\n");
}

// Function to find all hard links to a given file
void find_all_hlinks_in_path(const char* absolute_path_to_the_entry) {
    struct stat st;                 // Declare a structure to store file status
    if (lstat(absolute_path_to_the_entry, &st) == -1) { // Retrieve symbolic link status and check for failure
        perror("lstat");            // Print an error message
        exit(EXIT_FAILURE);         // Exit the program with a failure status
    }

    // Print absolute_path_to_the_entry file information
    printf("Hard links to %s (inode %lu):\n", absolute_path_to_the_entry, (unsigned long)st.st_ino);

    DIR *dir;                       // Declare a pointer to a directory stream
    struct dirent *entry;           // Declare a pointer to a directory entry

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {  // Iterate through the directory entries
        // exclude '.' and '. .' for current and prev directories
      if (entry->d_name[0] != '.') { // Exclude entries starting with a dot
            // Declare an array to store the full path of an entry
            char entry_path[PATH_MAX];
            // Construct the full path
            snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);
          //printf("%s\n", entry_path);


            struct stat entry_stat;
            if (lstat(entry_path, &entry_stat) == -1) {
                perror("lstat");
                exit(EXIT_FAILURE);
            }

            // Check for matching inode and different path
            if (entry_stat.st_ino == st.st_ino && strcmp(entry_path, absolute_path_to_the_entry) != 0) {
                printf("Hard link founded with following stat:\n\tInode: %lu, Path: %s\n",
                        (unsigned long)entry_stat.st_ino, entry_path);
            }
      }
    }
    printf("\n");
    closedir(dir);
}

// Function to unlink all duplicates of a hard link
// It keeps only one h.l.
void unlink_all_in_path(const char* absolute_path_to_the_entry) {
    struct stat st;                 // Declare a structure to store file status
    char * kept_link_name;          // Declare a pointer to store the name of the kept hard link
    int is_founded = 0;             // Declare a flag to indicate if the original file is found
    if (lstat(absolute_path_to_the_entry, &st) == -1) { // Retrieve symbolic link status and check for failure
        perror("lstat");            // Print an error message
        exit(EXIT_FAILURE);         // Exit the program with a failure status
    }

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        // Iterate through the directory entries
      char entry_path[PATH_MAX];
      snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);
      //printf("%s\n", entry_path);
        
        struct stat entry_stat;
        if (lstat(entry_path, &entry_stat) == -1) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        // Check for matching inode and different path
        if (entry_stat.st_ino == st.st_ino && strcmp(entry_path, absolute_path_to_the_entry) != 0) {
            if (unlink(entry_path) == -1) {
                // Unlink the entry and check for failure
                perror("unlink");
                exit(EXIT_FAILURE);
            }
            printf("Successefull unlink happen for: %s\n", entry_path);
        }
        // Check for matching inode and same path
        if (entry_stat.st_ino == st.st_ino && strcmp(entry_path, absolute_path_to_the_entry) == 0) {
            kept_link_name = entry->d_name;  // Store the name of the kept hard link
            is_founded = 1;                  // Set the flag to indicate the original file is found
        }
    }

    usleep(100);  // Introduce a delay to ensure proper execution
    if (is_founded == 1) {
        // Check if the original file is found
      printf("Stat info for the kept hard link:\n");
      print_stat_info_for_file(kept_link_name);
    } else {
      printf("It seems that original file you asked to keep is not in provided path. It wasn't removed");
    }
    
    printf("\n");
    closedir(dir);                  // Close the directory stream
}

// Function to create a symbolic link to a file in the watched directory
void create_sym_link(const char* absolute_path_to_the_entry, const char* link) {
    char link_path[PATH_MAX];
    snprintf(link_path, sizeof(link_path), "%s/%s", path, link);

    if (symlink(absolute_path_to_the_entry, link_path) == -1) {
        perror("symlink");
        exit(EXIT_FAILURE);
    }
    printf("Symbolic link {%s} created for absolute_path_to_the_entry %s\n\n", link_path, absolute_path_to_the_entry);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <watched_directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    path = argv[1];

    // Create myfile1.txt with content "Hello world."
    char myfile1_path[PATH_MAX];
    snprintf(myfile1_path, sizeof(myfile1_path), "%s/myfile1.txt", path);

    FILE *myfile1 = fopen(myfile1_path, "w");
    if (myfile1 == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    usleep(100);                    // Introduce a delay to ensure proper execution
    
    fprintf(myfile1, "Hello world.\n");
    usleep(100);                    // Introduce a delay to ensure proper execution
    
    fclose(myfile1);

    // Create hard links to myfile1.txt
    char myfile11_path[PATH_MAX];
    char myfile12_path[PATH_MAX];
    snprintf(myfile11_path, sizeof(myfile11_path),
             "%s/myfile11.txt", path);
    snprintf(myfile12_path, sizeof(myfile12_path),
             "%s/myfile12.txt", path);

    if (link(myfile1_path, myfile11_path) == -1 || link(myfile1_path, myfile12_path) == -1) {
        perror("link");
        exit(EXIT_FAILURE);
    }

    usleep(100);                    // Introduce a delay to ensure proper execution

    // Find and print all hard links to myfile1.txt
    find_all_hlinks_in_path(myfile1_path);
    usleep(100);                    // Introduce a delay to ensure proper execution

    // Move myfile1.txt to /tmp/myfile1.txt
    if (rename(myfile1_path, "/tmp/myfile1.txt") == -1) {
        perror("rename");
        exit(EXIT_FAILURE);
    }
    usleep(100);                    // Introduce a delay to ensure proper execution

    // Modify myfile11.txt
    FILE *myfile11 = fopen(myfile11_path, "a");
    if (myfile11 == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    usleep(100);                    // Introduce a delay to ensure proper execution
    
    fprintf(myfile11, "Modification.");
    usleep(100);                    // Introduce a delay to ensure proper execution
    fclose(myfile11);
    

    // Create symbolic link myfile13.txt to /tmp/myfile1.txt
    create_sym_link("/tmp/myfile1.txt", "myfile13.txt");
    usleep(100);                    // Introduce a delay to ensure proper execution

    // Modify /tmp/myfile1.txt
    FILE *tmpfile1 = fopen("/tmp/myfile1.txt", "a");
    if (tmpfile1 == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    usleep(100);                    // Introduce a delay to ensure proper execution
    
    fprintf(tmpfile1, "Modification in /tmp/myfile1.txt.");
    usleep(100);                    // Introduce a delay to ensure proper execution
    fclose(tmpfile1);


    // Remove duplicates of hard links to myfile11.txt
    unlink_all_in_path(myfile11_path);
    usleep(100);                    // Introduce a delay to ensure proper execution


    return 0;
}
