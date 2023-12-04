#!/bin/bash

# Compile the C program
gcc ex2.c -o bankers_algorithm

# Run the compiled program with the input file
./bankers_algorithm input.txt

# Clean up the compiled program
rm bankers_algorithm
