#include <stdio.h>

void convert(long long x, int s, int t) {
    // Check if s and t are within the range [2, 10]
    if (s < 2 || s > 10 || t < 2 || t > 10) {
        printf("cannot convert!\n");
        return;
    }

    // Convert x from numeral system s to decimal
    long long decimalValue = 0;
    int base = 1;

    while (x > 0) {
        long long digit = x % 10;
        if (digit >= s) {
            printf("cannot convert!\n");
            return;
        }
        decimalValue += digit * base;
        base *= s;
        x /= 10;
    }

    char result[65];
    int resultDigit = 64;
    while(decimalValue > 0) {
        int digitInt = decimalValue % t;
        char digitChar = digitInt + '0';
        result[resultDigit] = digitChar;
        decimalValue = decimalValue / t;
        resultDigit--;
    }

    for(int i = resultDigit+1; i < 65; i++) {
        printf("%c", result[i]);
    }
    printf("\n");
}

int main(void) {
    printf("Enter a initial number, source and target number system in a following format:\n");
    printf("initial_number source target\n");
    long long num;
    int source;
    int target;
    scanf("%lld %d %d", &num, &source, &target);
    convert(num, source, target);

    return 0;
}
