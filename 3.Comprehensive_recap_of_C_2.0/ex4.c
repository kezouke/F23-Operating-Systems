#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>

// Enum to indicate the data type
typedef enum {
    INT,
    DOUBLE
} DataType;

// Struct to store the value and data type
typedef struct {
    DataType type;
    union {
        int i;
        double d;
    } value;
} Number;

// Addition operation for integers and doubles
void *add(const void *a, const void *b) {
    if (a == NULL || b == NULL) return NULL;

    const Number *num1 = (const Number *)a;
    const Number *num2 = (const Number *)b;

    Number *result = malloc(sizeof(Number));
    if (num1->type == INT) {
        result->type = INT;
        result->value.i = num1->value.i + num2->value.i;
    } else {
        result->type = DOUBLE;
        result->value.d = num1->value.d + num2->value.d;
    }

    return result;
}

// Multiplication operation for integers and doubles
void *multiply(const void *a, const void *b) {
    if (a == NULL || b == NULL) return NULL;

    const Number *num1 = (const Number *)a;
    const Number *num2 = (const Number *)b;

    Number *result = malloc(sizeof(Number));
    if (num1->type == INT) {
        result->type = INT;
        result->value.i = num1->value.i * num2->value.i;
    } else {
        result->type = DOUBLE;
        result->value.d = num1->value.d * num2->value.d;
    }

    return result;
}

// Max operation for integers and doubles
void *max(const void *a, const void *b) {
    if (a == NULL || b == NULL) return NULL;

    const Number *num1 = (const Number *)a;
    const Number *num2 = (const Number *)b;

    Number *result = malloc(sizeof(Number));
    if (num1->type == INT) {
        result->type = INT;
        result->value.i = (num1->value.i > num2->value.i) ? num1->value.i : num2->value.i;
    } else {
        result->type = DOUBLE;
        result->value.d = (num1->value.d > num2->value.d) ? num1->value.d : num2->value.d;
    }

    return result;
}

// Generic aggregation function
void *aggregate(void *base, size_t size, int n, void *initial_value,
                void *(*opr)(const void *, const void *)) {
    if (base == NULL || initial_value == NULL || opr == NULL) return NULL;

    Number *result = malloc(sizeof(Number));
    result->type = (size == sizeof(int)) ? INT : DOUBLE;

    if (size == sizeof(int)) {
        result->value.i = *((int *) initial_value);
    } else {
        result->value.d = *((double *) initial_value);
    }

    for (int i = 0; i < n; i++) {
        Number *current = malloc(sizeof(Number));
        current->type = (size == sizeof(int)) ? INT : DOUBLE;

        if (size == sizeof(int)) {
            current->value.i = *((int *) (base + i * size));
        } else {
            current->value.d = *((double *) (base + i * size));
        }

        Number *temp_result = (Number *) opr(result, current);
        free(result);
        free(current);
        result = temp_result;
    }

    return result;
}

int main() {
    // Test on an array of 5 doubles
    double doubleArray[5] = {2.5, 1.4, 1.7, 4.2, 5.9};
    double initialDoubleValue = 0.0;
    Number *sumDoubleResult = (Number *) aggregate(doubleArray, sizeof(double), 5, &initialDoubleValue, add);
    initialDoubleValue = 1.0;
    Number *productDoubleResult = (Number *) aggregate(doubleArray, sizeof(double), 5, &initialDoubleValue, multiply);
    initialDoubleValue = -DBL_MAX;
    Number *maxDoubleResult = (Number *) aggregate(doubleArray, sizeof(double), 5, &initialDoubleValue, max);

    printf("Sum of doubles: %.2lf\n", (sumDoubleResult->type == DOUBLE) ? sumDoubleResult->value.d : sumDoubleResult->value.i);
    printf("Product of doubles: %.2lf\n", (productDoubleResult->type == DOUBLE) ? productDoubleResult->value.d : productDoubleResult->value.i);
    printf("Max of doubles: %.2lf\n", (maxDoubleResult->type == DOUBLE) ? maxDoubleResult->value.d : maxDoubleResult->value.i);
    

    // Test on an array of 5 integers
    int intArray[5] = {3, 5, 1, 71, 2};
    int initialIntValue = 0;
    Number *sumIntResult = (Number *) aggregate(intArray, sizeof(int), 5, &initialIntValue, add);
    initialIntValue = 1;
    Number *productIntResult = (Number *) aggregate(intArray, sizeof(int), 5, &initialIntValue, multiply);
    initialIntValue = INT_MIN;
    Number *maxIntResult = (Number *) aggregate(intArray, sizeof(int), 5, &initialIntValue, max);

    printf("Sum of integers: %d\n", sumIntResult->value.i);
    printf("Product of integers: %d\n", productIntResult->value.i);
    printf("Max of integers: %d\n", maxIntResult->value.i);

    free(sumDoubleResult);
    free(productDoubleResult);
    free(maxDoubleResult);
    free(sumIntResult);
    free(productIntResult);
    free(maxIntResult);

    return 0;
}
