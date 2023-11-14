#!/bin/bash

# Create a temporary directory named "tmp"
mkdir tmp

# Store the current working directory in a variable
current_directory=$(pwd)

# Compile monitor.c and generate an executable named "monitor"
gcc -o monitor monitor.c

# Compile ex1.c and generate an executable named "ex1"
gcc -o ex1 ex1.c

# Run monitor.c in the background, monitoring the "tmp" directory
./monitor "$current_directory/tmp" &

# Pause the script for 3 seconds to allow monitor.c to initialize
sleep 3

# Run ex1.c in a new GNOME Terminal window, passing the "tmp" directory as an argument
gnome-terminal -- bash -c "./ex1 $current_directory/tmp; exec bash"

# Change the working directory to "tmp"
cd "$current_directory/tmp"

# Run the ex1_test.sh script within the "tmp" directory
bash "$current_directory/ex1_test.sh"

# Change the working directory back to the original directory
cd "$current_directory"

# Gracefully terminate monitor.c using Ctrl+C signal
pkill -INT monitor

# Pause the script for 3 seconds to allow monitor.c to terminate
sleep 3

# Clean up compiled files (monitor and ex1) and the temporary directory "tmp"
rm monitor ex1
rm -r tmp
