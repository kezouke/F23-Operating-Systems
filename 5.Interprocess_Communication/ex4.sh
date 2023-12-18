#!/bin/bash

gcc ex4.c -o ex4 -lpthread

m_values=(1 2 4 10 100)

echo -n > ex4_res.txt

for m in "${m_values[@]}"
do
    { time ./ex4 10000000 $m; } 2>> ex4_res.txt
done

rm ex4

echo "Execution times saved in ex4_res.txt"
