#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SIZE 120


int dot_product(int u[], int v[], int s, int f) {
    int result = 0;
    for(int i = s; i < f; i++) {
    	result += u[i] * v[i];
    }
    return result;
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    if (n < 0 || n > 10 || n % 2 != 0) {
    	printf("Invalid number was entered :(\n");
    	return 1;
    }
    
    srand(time(NULL));
    
    int u[SIZE];
    int v[SIZE];
    
    for (int i = 0; i < SIZE; i++) {
    	u[i] = rand() % 100;
    	v[i] = rand() % 100;
    }
    
    printf("\n\nFirst vector u: ");
    for (int i = 0; i < SIZE; i++){
    	printf("%d ", u[i]);
    }
    printf("\n\nSecond vector v: ");
    for (int i = 0; i < SIZE; i++) {
    	printf("%d ", v[i]);
    }
    printf("\n");
    
    FILE *file = fopen("temp.txt", "w+");
    if (file == NULL) {
    	printf("Error while opening file\n");
    	return 1;
    }
    
    pid_t processes[n];

    
    for(int i = 0; i < n; i++) {
    	int start = i * SIZE / n;
    	int finish = (i + 1) * SIZE / n;
    	
    	processes[i] = fork();
    	if (processes[i] == 0) {
    	    int res = dot_product(u, v, start, finish);
    	    fprintf(file, "%d\n", res);
    	    fclose(file);
    	    exit(0);
    	}
    }
    
    for(int i = 0; i < n; i++) {
    	wait(NULL);
    }
    
    
    fseek(file, 0, SEEK_SET);
    int dot_product_result = 0;
    for(int i = 0; i < n; i++) {
    	int res;
    	fscanf(file, "%d", &res);
    	dot_product_result += res;
    }
    
    printf("Dot procduct: %d\n", dot_product_result);
    
    return 0;
}
