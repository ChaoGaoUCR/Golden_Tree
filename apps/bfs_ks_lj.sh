#!/bin/bash
make BFS
for ((a=70000; a<=70000*20; a+=70000))
do
for ((i=0; i<5; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
    echo "----------------------------------------------"    
done

done