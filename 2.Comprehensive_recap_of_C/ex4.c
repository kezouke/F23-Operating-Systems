#include <stdio.h>
#include <string.h>
#include <ctype.h> // For tolower function

int count(char *str, char target, int len) {
    int count = 0;
    target = tolower(target);
    for (int i = 0; i < len; i++) {
        char current = tolower(str[i]);
        if (current == target) {
            count++;
        }
    }

    return count;
}

void countAll(char str[]) {
    int len = strlen(str);
    if (str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }

    for (int i = 0; i < len; i++) {
        char current = str[i];

        if (current != '\0') {
            int charCount = count(str, current, len);

            printf("%c:%d, ", tolower(current), charCount);

            for (int j = i; j < len; j++) {
                if (tolower(str[j]) == tolower(current)) {
                    str[j] = '\0';
                }
            }
        }
    }

    printf("\n");
}

int main() {
    char inputString[256];

    // Read the input string
    printf("Enter a string: ");
    fgets(inputString, sizeof(inputString), stdin);
    countAll(inputString);

    return 0;
}
