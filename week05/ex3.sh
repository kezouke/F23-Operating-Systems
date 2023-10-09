#!/bin/bash

gcc -pthread ex3.c -o ex3

m_values=(1 2 4 10 100)

echo -n > ex3_res.txt

for m in "${m_values[@]}"
do
    { time ./ex3 10000000 $m; } 2>> ex3_res.txt
done

rm ex3

echo "Execution times saved in ex3_res.txt"
