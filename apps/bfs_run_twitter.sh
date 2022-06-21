#!/bin/bash
make BFS
for ((a=150000; a<=150000*2; a+=150000))
do
for ((i=0; i<5; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    echo "----------------------------------------------"    
done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done