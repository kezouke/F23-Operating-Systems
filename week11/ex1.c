#include <stdio.h>
#include <dirent.h>

int main() {
    // Declare a pointer to the directory structure and a structure for directory entries
    DIR *d;
    struct dirent *dir;

    // Open the root directory "/"
    d = opendir("/");
    
    // Check if the directory was successfully opened
    if (d) {
        // Iterate through each directory entry and print its name
        while ((dir = readdir(d)) != NULL) {
            // Print the name of the directory entry
            printf("%s\n", dir->d_name);
        }

        // Close the directory
        closedir(d);
    }

    // Return 0 to indicate successful execution
    return 0;
}
