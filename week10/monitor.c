// monitor.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/inotify.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define EVENT_BUF_LEN     (1024 * (EVENT_SIZE + 16))

char *path;
int fd;
int wd;
char buffer[EVENT_BUF_LEN];

// Function to print the stat information for a given entry
void print_stat_info_for_file(const char *entry_name) {
    char full_path[PATH_MAX];
    sprintf(full_path, "%s/%s", path, entry_name);

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


// Function to print stat info for all entries in the directory
void print_stat_for_all_files() {
    // Traverse the directory and print stat info for all entries
    printf("Stat for all files:\n");
    int number_of_files = 0;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        closedir(dir);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0] != '.') {
          if (entry->d_type == DT_REG || entry->d_type == DT_DIR) {
                print_stat_info_for_file(entry->d_name);
                number_of_files += 1;
            }
      }
        
    }
    if (number_of_files == 0) {
      printf("No files in directory yet\n");
    }
    closedir(dir);
}

// Signal handler for SIGINT to print stat before termination
void sigintHandler(int sig_num) {
    printf("\nReceived SIGINT. Printing stat info before termination:\n");
    if (fd) {
        inotify_rm_watch(fd, wd);
        close(fd);
    }
    print_stat_for_all_files();
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    // Check for correct command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    path = argv[1];
    signal(SIGINT, sigintHandler);

    // Initialize inotify
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
    }

    // Add a watch for specified directory and events
    wd = inotify_add_watch(fd, path, IN_ACCESS | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN | IN_ATTRIB);
    if (wd == -1) {
        printf("Could not watch : %s\n", argv[1]);
    }

    // Print initial stat information for all files
    print_stat_for_all_files();

    printf("\nMonitoring %s for changes...\n", path);
    while (1) {
        int i = 0;
        int length = read(fd, buffer, EVENT_BUF_LEN);

        if (length < 0) {
            perror("read");
        }

        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR) {
                        printf("New directory %s created.\n", event->name);
                    } else {
                        printf("New file %s created.\n", event->name);
                    }
                } else if (event->mask & IN_DELETE) {
                    if (event->mask & IN_ISDIR) {
                        printf("Directory %s deleted.\n", event->name);
                    } else {
                        printf("File %s deleted.\n", event->name);
                    }
                } else if (event->mask & IN_MODIFY) {
                    if (event->mask & IN_ISDIR) {
                        printf("Directory %s modified.\n", event->name);
                    } else {
                        printf("File %s modified.\n", event->name);
                    }
                } else if (event->mask & IN_ACCESS) {
                    if (event->mask & IN_ISDIR) {
                        printf("Directory %s accessed.\n", event->name);
                    } else {
                        printf("File %s accessed.\n", event->name);
                    }
                } else if (event->mask & IN_OPEN) {
                    if (event->mask & IN_ISDIR) {
                        printf("Directory %s was opened.\n", event->name);
                    } else {
                        printf("File %s was opened.\n", event->name);
                    }
                } else if (event->mask & IN_ATTRIB) {
                    if (event->mask & IN_ISDIR) {
                        printf("Metadata of directory %s changed.\n", event->name);
                    } else {
                        printf("Metadata of file %s changed.\n", event->name);
                    }
                }
                print_stat_info_for_file(event->name);
            }
            i += EVENT_SIZE + event->len;
        }
    }
}
