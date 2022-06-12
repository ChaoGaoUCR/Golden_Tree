#!/bin/bash
make SSSP
for ((a=70000; a<=70000*50; a+=70000))
do
for ((i=0; i<5; i ++))
do
    ./SSSP -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
done
echo "----------------------------------------------"
done