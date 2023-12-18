#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

#define KEYBOARD_PATH "/dev/input/by-path/platform-i8042-serio-0-event-kbd"
#define OUTPUT_FILE "ex1.txt"

void printMessage(FILE *file, const char *message) {
    printf("%s\n", message);
    fprintf(file, "%s\n", message);
}

int main() {
    // Define the path to the keyboard device
    char* keyboardPath = KEYBOARD_PATH;

    // Define the input_event structure to store information about keyboard events
    struct input_event keyEvent;

    // File descriptor for the keyboard device
    int keyboardFileDescriptor;

    // File pointer for the output file
    FILE *outputFile;

    // Define the map for translating key event values to human-readable strings
    char *keyEventMap[] = {"RELEASED", "PRESSED ", "REPEATED"};

    // Initialize flags to track whether specific keys are pressed
    int isPKeyPressed = 0; // Flag for the 'P' key
    int isEKeyPressed = 0; // Flag for the 'E' key
    int isXKeyPressed = 0; // Flag for the 'X' key
    int isCKeyPressed = 0; // Flag for the 'C' key
    int isAKeyPressed = 0; // Flag for the 'A' key
    int isCapsLockKeyPressed = 0; // Flag for the Caps Lock key
    int isBKeyPressed = 0; // Flag for the 'B' key
    int isMKeyPressed = 0; // Flag for the 'M' key
    int isWKeyPressed = 0; // Flag for the 'W' key

    // Open the keyboard device file for reading
    keyboardFileDescriptor = open(keyboardPath, O_RDONLY);
    if (keyboardFileDescriptor == -1) {
        // Print an error message if the file cannot be opened
        fprintf(stderr, "Error: Could not open the keyboard device file: %s\n", keyboardPath);
        return EXIT_FAILURE;
    }

    // Open the output file in append mode
    outputFile = fopen(OUTPUT_FILE, "a");
    if (outputFile == NULL) {
        // Print an error message if the file cannot be opened
        fprintf(stderr, "Error: Could not open the output file (ex1.txt)\n");
        return EXIT_FAILURE;
    }

    // Main loop to continuously read keyboard events
    while (1) {
        // Read the next keyboard event into the input_event structure
        read(keyboardFileDescriptor, &keyEvent, sizeof(struct input_event));

        // Check if the event is a key event and has a valid value
        if (keyEvent.type == EV_KEY && (keyEvent.value == 0 || keyEvent.value == 1 || keyEvent.value == 2)) {
            // Print the key event to the console and the output file
            fprintf(stdout, "Event: %s, Key Code: 0x%04x (%d)\n", keyEventMap[keyEvent.value], (int)keyEvent.code, (int)keyEvent.code);
            fprintf(outputFile, "Event: %s, Key Code: 0x%04x (%d)\n", keyEventMap[keyEvent.value], (int)keyEvent.code, (int)keyEvent.code);

            // Switch case to determine which key is pressed and update flags accordingly
            switch (keyEvent.code) {
                case KEY_P:
                    isPKeyPressed = keyEvent.value;
                    break;
                case KEY_E:
                    isEKeyPressed = keyEvent.value;
                    break;
                case KEY_C:
                    isCKeyPressed = keyEvent.value;
                    break;
                case KEY_A:
                    isAKeyPressed = keyEvent.value;
                    break;
                case KEY_CAPSLOCK:
                    isCapsLockKeyPressed = keyEvent.value;
                    break;
                case KEY_B:
                    isBKeyPressed = keyEvent.value;
                    break;
                case KEY_M:
                    isMKeyPressed = keyEvent.value;
                    break;
                case KEY_W:
                    isWKeyPressed = keyEvent.value;
                    break;
                case KEY_X:
                    isXKeyPressed = keyEvent.value;
		    break;
                default:
		    // if any other key is pressed -> reset variables
		    isPKeyPressed = 0; // Flag for the 'P' key
		    isEKeyPressed = 0; // Flag for the 'E' key
		    isXKeyPressed = 0; // Flag for the 'X' key
		    isCKeyPressed = 0; // Flag for the 'C' key
		    isAKeyPressed = 0; // Flag for the 'A' key
		    isCapsLockKeyPressed = 0; // Flag for the Caps Lock key
		    isBKeyPressed = 0; // Flag for the 'B' key
		    isMKeyPressed = 0; // Flag for the 'M' key
		    isWKeyPressed = 0; // Flag for the 'W' key
                    break;
            }

            // Check for specific key combinations and print corresponding messages
            // IMPORTANT NOTE:
            // THE CHECKING FOR ALL OTHER SYMBOLS IS NEEDED IN CASES
            // WHEN USER FOR EXAMPLE ENTER C + W + A + P
            // SINCE IT IS ANOTHER SHORTCUT, WE HAVE TO MAKE ALL FLAGS TO BE ZERO
            // AND ALSO CHECK THAT ANOTHER FLAG IS NOT PRESSED
            // IN ORDER NOT TO HAVE C + B + A + P
            if (isPKeyPressed && isEKeyPressed && !isXKeyPressed
            	&& !isCKeyPressed && !isAKeyPressed && !isCapsLockKeyPressed
            	&& !isBKeyPressed && !isMKeyPressed && !isWKeyPressed) {
                // Print message for the 'P' and 'E' keys combination
                printMessage(outputFile, "Shortcut Detected: 'P' + 'E'");
                printMessage(outputFile, "Message: I passed the Exam!");
            }   

            if (isPKeyPressed && !isEKeyPressed && !isXKeyPressed
            	&& isCKeyPressed && isAKeyPressed && !isCapsLockKeyPressed
            	&& !isBKeyPressed && !isMKeyPressed && !isWKeyPressed) {
                // Print message for the 'C', 'A', and 'P' keys combination
                printMessage(outputFile, "Shortcut Detected: 'C' + 'A' + 'P'");
                printMessage(outputFile, "Message: Get some cappuccino!");
            }

            if (!isPKeyPressed && !isEKeyPressed && !isXKeyPressed
            	&& !isCKeyPressed && !isAKeyPressed && !isCapsLockKeyPressed
            	&& isBKeyPressed && isMKeyPressed && isWKeyPressed) {
                // Print message for the 'B', 'M', and 'W' keys combination
                printMessage(outputFile, "Shortcut Detected: 'B' + 'M' + 'W'");
                printMessage(outputFile, "Message: BMW is the best car company ever");
            }

            // Check if the keys 'X' and 'E' were pressed to exit the loop
            if (!isPKeyPressed && isEKeyPressed && isXKeyPressed
            	&& !isCKeyPressed && !isAKeyPressed && !isCapsLockKeyPressed
            	&& !isBKeyPressed && !isMKeyPressed && !isWKeyPressed) {
                printMessage(outputFile, "Exit Shortcut Detected: 'X' + 'E'");
                break;
            }
        }
    }

    // Close the output file and the keyboard device
    fclose(outputFile);
    close(keyboardFileDescriptor);

    return EXIT_SUCCESS;
}
