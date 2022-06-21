#!/bin/bash
make BFS
echo "Live-J start"
for ((a=70000; a<=70000*100; a+=70000))
do
for ((i=0; i<5; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
done

done
    echo "----------------------------------------------"    
