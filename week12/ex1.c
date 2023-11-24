#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

int main() {
    // Define the path to the keyboard device
    char *path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    
    // Define the input_event structure to store information about keyboard events
    struct input_event ev;
    // File descriptor for the keyboard device
    int fd;
    // File pointer for the output file
    FILE *file;
    
    // Define the map for translating key event values to human-readable strings
    char *map[] = {"RELEASED", "PRESSED ", "REPEATED"};
    
    // Initialize flags to track whether specific keys are pressed
    int p_pressed = 0;  // Flag for the 'P' key
    int e_pressed = 0;  // Flag for the 'E' key
    int x_pressed = 0;  // Flag for the 'X' key
    int c_pressed = 0;  // Flag for the 'C' key
    int a_pressed = 0;  // Flag for the 'A' key
    int cap_pressed = 0;  // Flag for the Caps Lock key
    int b_pressed = 0;  // Flag for the 'B' key
    int m_pressed = 0;  // Flag for the 'M' key
    int w_pressed = 0;  // Flag for the 'W' key

    // Open the keyboard device file for reading
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        // Print an error message if the file cannot be opened
        fprintf(stderr, "Error: Could not open the keyboard device file: %s\n", path);
        return EXIT_FAILURE;
    }

    // Open the output file in append mode
    file = fopen("ex1.txt", "a");
    if (file == NULL) {
        // Print an error message if the file cannot be opened
        fprintf(stderr, "Error: Could not open the output file (ex1.txt)\n");
        return EXIT_FAILURE;
    }

    // Main loop to continuously read keyboard events
    while (1) {
        // Read the next keyboard event into the input_event structure
        read(fd, &ev, sizeof(struct input_event));

        // Check if the event is a key event and has a valid value
        if (ev.type == EV_KEY && (ev.value == 0 || ev.value == 1 || ev.value == 2)) {
            // Print the key event to the console and the output file
            fprintf(stdout, "Event: %s, Key Code: 0x%04x (%d)\n", map[ev.value], (int)ev.code, (int)ev.code);
            fprintf(file, "Event: %s, Key Code: 0x%04x (%d)\n", map[ev.value], (int)ev.code, (int)ev.code);

            // Switch case to determine which key is pressed and update flags accordingly
            switch (ev.code) {
                case KEY_P:
                    p_pressed = ev.value;
                    break;
                case KEY_E:
                    e_pressed = ev.value;
                    break;
                case KEY_C:
                    c_pressed = ev.value;
                    break;
                case KEY_A:
                    a_pressed = ev.value;
                    break;
                case KEY_CAPSLOCK:
                    cap_pressed = ev.value;
                    break;
                case KEY_B:
                    b_pressed = ev.value;
                    break;
                case KEY_M:
                    m_pressed = ev.value;
                    break;
                case KEY_W:
                    w_pressed = ev.value;
                    break;
                case KEY_X:
                    x_pressed = ev.value;
                default:
                    break;
            }

            // Check for specific key combinations and print corresponding messages
            if (p_pressed && e_pressed) {
                // Print message for the 'P' and 'E' keys combination
                printf("Shortcut Detected: 'P' + 'E'\n");
                printf("Message: I passed the Exam!\n");
                fprintf(file, "Shortcut Detected: 'P' + 'E'\n");
                fprintf(file, "Message: I passed the Exam!\n");
            }

            if (c_pressed && a_pressed && p_pressed) {
                // Print message for the 'C', 'A', and 'P' keys combination
                printf("Shortcut Detected: 'C' + 'A' + 'P'\n");
                printf("Message: Get some cappuccino!\n");
                fprintf(file, "Shortcut Detected: 'C' + 'A' + 'P'\n");
                fprintf(file, "Message: Get some cappuccino!\n");
            }

            if (b_pressed && m_pressed && w_pressed) {
                // Print message for the 'B', 'M', and 'W' keys combination
                printf("Shortcut Detected: 'B' + 'M' + 'W'\n");
                printf("Message: BMW is the best car company ever\n");
                fprintf(file, "Shortcut Detected: 'B' + 'M' + 'W'\n");
                fprintf(file, "Message: BMW is the best car company ever\n");
            }

            // Check if the keys 'X' and 'E' were pressed to exit the loop
            if (x_pressed && e_pressed) {
                // Print message and exit the program
                printf("Exit Shortcut Detected: 'X' + 'E'\n");
                fprintf(file, "Exit Shortcut Detected: 'X' + 'E'\n");
                break;
            }
        }
    }

    // Close the output file and the keyboard device
    fclose(file);
    close(fd);

    return EXIT_SUCCESS;
}
