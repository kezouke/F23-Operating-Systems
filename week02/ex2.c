#include <stdio.h>
#include <string.h>

int main(void) {
    char input[257]; // +1 for /0 char
    char character;
    int i = 0;
    
    printf("Enter a string char by char (256 chars at maximum):\n");
    
    while ((character = getchar()) != '.' && i < 256) {
    	input[i] = character;
    	i++;
    }
    
    int length = strlen(input);
    input[length] = '\0';
    
    printf("Reversed string:\n");	
    for(int i = length; i >= 0; i--) {
    	printf("%c", input[i]);
    }
    printf("\n");
    
    
    return 0;
}

