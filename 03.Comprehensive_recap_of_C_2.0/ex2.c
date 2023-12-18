#include <stdio.h>
#include <math.h>

// Define a structure for Point
struct Point {
    long double x;
    long double y;
};

// Function to calculate the distance between two points
long double distance(struct Point p1, struct Point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// Function to calculate the area of a triangle using Heron's formula
long double area(struct Point a, struct Point b, struct Point c) {
    return fabsl(a.x * b.y - b.x * a.y + b.x * c.y - c.x * b.y + c.x * a.y - a.x * c.y) / 2.0;
}

int main() {
    // Define points A, B, and C
    struct Point A = {2.5, 6};
    struct Point B = {1, 2.2};
    struct Point C = {10, 6};

    // Calculate the distance between points A and B
    long double distAB = distance(A, B);

    // Calculate the area of triangle ABC
    long double triangleArea = area(A, B, C);

    // Print the results
    printf("Distance between A and B: %Lf\n", distAB);
    printf("Area of triangle ABC: %Lf\n", triangleArea);

    return 0;
}
