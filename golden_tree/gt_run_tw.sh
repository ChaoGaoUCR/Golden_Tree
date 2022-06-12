#!/bin/bash
make SSSP
for ((a=150000; a<=150000*50; a+=150000))
do
for ((i=0; i<5; i ++))
do
    ./SSSP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
done
echo "----------------------------------------------"
done