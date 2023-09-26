#include <stdio.h>
#include <string.h>

#define MAX_NAME 64 // +1 for null character
#define MAX_PATH 2048
#define MAX_DATA 1024

// Define a struct File
typedef struct File {
    unsigned long long int id;
    char name[MAX_NAME];
    unsigned long long int size;
    char data[MAX_DATA];
    struct Directory *directory; // The parent directory
} File;

// Define a struct Directory
typedef struct Directory {
    unsigned char nf;
    unsigned char nd;
    struct File files[255];
    struct Directory *directories[255];
    char path[MAX_PATH];
    char name[MAX_NAME];
} Directory;

// Prints the name of the File
void show_file(File *file) {
    printf("%s ", file->name);
}

// Displays the content of the Directory dir
void show_dir(Directory *dir) {
    printf("\nDIRECTORY\n");
    printf(" path: %s\n", dir->path);
    printf(" files:\n");
    printf(" [ ");
    for (int i = 0; i < (int) dir->nf; i++) {
        show_file(&(dir->files[i]));
    }
    printf("]\n");
    printf(" directories:\n");
    printf(" { ");
    for (int i = 0; i < (int) dir->nd; i++) {
        show_dir(dir->directories[i]);
    }
    printf("}\n");
}

// Adds the subdirectory dir1 to the directory dir2
void add_dir(Directory *dir1, Directory *dir2) {
    if (dir1 && dir2) {
        dir2->directories[(int) dir2->nd] = dir1;
        dir2->nd++;
        char temp_path[MAX_PATH];
        if (strcmp(dir2->path, "/")) {
            strcpy(temp_path, dir2->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        } else {
            strcpy(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
    }
}

// Function to print the path of a file
void printp_file(struct File *file) {
    printf("%s/", file->directory->path);
    show_file(file);
    printf("\n");
}

// Function to overwrite the content of a file
void overwrite_to_file(struct File *file, const char *str) {
    strncpy(file->data, str, MAX_DATA);
    file->size = strlen(file->data) + 1; // Include the null terminator
}

// Function to append content to a file
void append_to_file(struct File *file, const char *str) {
    strncat(file->data, str, MAX_DATA - strlen(file->data) - 1); // -1 for the null terminator
    file->size = strlen(file->data) + 1; // Update the size
}

// Function to add a new file to the current directory
void add_file(struct File* file, struct Directory* dir) {
    if (dir->nf < 256) {
        file->directory = dir;
        dir->files[dir->nf] = *file;
        dir->nf++;
    } else {
        printf("Cannot add more files to the directory. Maximum limit reached.\n");
    }
}


int main() {
    // Create the root directory
    Directory root = {
            .nf = 0,
            .nd = 0,
            .path = "/",
            .name = "root"
    };

    // Create the home directory
    Directory home = {
            .nf = 0,
            .nd = 0,
            .path = "",
            .name = "home"
    };

    // Create the bin directory
    Directory bin = {
            .nf = 0,
            .nd = 0,
            .path = "",
            .name = "bin"
    };
    add_dir(&bin, &root);
    add_dir(&home, &root);

    // Add a file "bash" to the bin directory
    File bash = {
            .id = 1,
            .name = "bash",
            .size = 0,
            .data = "",
    };
    add_file(&bash, &bin);

    // Add two files to the home directory
    File ex3_1 = {
            .id = 2,
            .name = "ex3_1.c",
            .size = 0,
            .data = "",
    };
    overwrite_to_file(&ex3_1, "int printf(const char * format, ...);");
    File ex3_2 = {
            .id = 3,
            .name = "ex3_2.c",
            .size = 0,
            .data = "",
    };
    overwrite_to_file(&ex3_2, "//This is a comment in C language");
    add_file(&ex3_1, &home);
    add_file(&ex3_2, &home);

    // Overwrite the content of the "bash" file
    overwrite_to_file(&bash, "Bourne Again Shell!!");
    

    // Append content to the "ex3_1.c" file
    append_to_file(&ex3_1, "int main(){printf(\"Hello World!\");}");

    // Print the path of all files
    printf("Paths of all files:\n");
    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);

    show_dir(&root);

    return 0;
}

