#!/bin/bash
# make BFS SSSP SSWP SSNP

echo "***********BFS starts***********"

for ((i=0; i<5; i ++))
do
    ./BFS -Snapshots 50 -E_number 75000 ../Twitter/twitter.adj
    # echo "----------------------------------------------"    
done
echo "***********SSSP starts***********"

for ((i=0; i<5; i ++))
do
    ./SSSP -Snapshots 50 -E_number 75000 ../Twitter/twitter.adj
    # echo "----------------------------------------------"    
done
echo "***********SSWP starts***********"
for ((i=0; i<5; i ++))
do
    ./SSWP -Snapshots 50 -E_number 75000 ../Twitter/twitter.adj
    # echo "----------------------------------------------"    
done
echo "***********SSNP starts***********"
for ((i=0; i<5; i ++))
do
    ./SSNP -Snapshots 50 -E_number 75000 ../Twitter/twitter.adj
    # echo "----------------------------------------------"    
done