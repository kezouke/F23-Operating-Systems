#!/bin/bash

# Create the directory if it doesn't exist
mkdir -p /tmp/ex2

# Create an empty file for the page table
touch /tmp/ex2/pagetable

# Compile pager.c and mmu.c
gcc pager.c -o pager 
gcc mmu.c -o mmu

# Run pager with 4 pages and 2 frames 
./pager 4 2 &

# Capture the process ID of the pager
pid_pager=$!

sleep 3

gnome-terminal -- bash -c "./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pid_pager; exec bash"

# Run mmu with the reference string in a new terminal window

# Clean up
rm pager mmu

# Wait for both processes to finish
wait

rm -r /tmp/ex2
