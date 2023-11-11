#!/bin/bash


mkdir tmp

# Compile monitor.c
gcc -o monitor monitor.c

# Compile ex1.c
gcc -o ex1 ex1.c

# Run monitor.c in the background
./monitor /home/kezouke/week10/tmp &

sleep 1

# Run ex1.c
gnome-terminal -- bash -c "./ex1 /home/kezouke/week10/tmp; exec bash"

cd ~/week10/tmp
bash ~/week10/ex1_test.sh

cd ../

# Terminate monitor.c gracefully with Ctrl+C
kill -INT %%

sleep 3

# Clean up compiled files
rm monitor ex1

rm -r tmp
