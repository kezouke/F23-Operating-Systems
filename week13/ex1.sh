#!/bin/bash

# Compile the C code
gcc -o ex1 ex1.c -lpthread

# Run the executable with command line arguments
./ex1 4 4

# Cleanup the compiled executable
rm ex1
