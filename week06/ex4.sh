gcc worker.c -o worker
gcc scheduler_rr.c -o scheduler_rr

./scheduler_rr data.txt

rm worker
rm scheduler_rr


