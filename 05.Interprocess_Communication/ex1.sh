#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number_of_subscribers>"
    exit 1
fi

n=$1

if [ $n -le 0 ] || [ $n -ge 4 ]; then
    echo "Number of subscribers must be between 1 and 3"
    exit 1
fi

mkdir -p /tmp/ex1

gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

for ((i=1; i<=$n; i++)); do
    gnome-terminal -- bash -c "./subscriber $i; exec bash" &
done

./publisher $n

wait

rm publisher
rm subscriber

