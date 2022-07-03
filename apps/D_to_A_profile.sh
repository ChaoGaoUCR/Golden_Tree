#!/bin/bash
for ((a=35000; a<=35000*5; a+=35000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
    ./SSSP -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
    ./SSWP -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
    ./SSNP -Snapshots 1 -E_number $a ../LiveJ/LJ.adj

    echo "----------------------------------------------"    
done

done

echo "===================================================="
for ((a=40000; a<=40000*5; a+=40000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../wiki/wiki.adj
    ./SSSP -Snapshots 1 -E_number $a ../wiki/wiki.adj
    ./SSWP -Snapshots 1 -E_number $a ../wiki/wiki.adj
    ./SSNP -Snapshots 1 -E_number $a ../wiki/wiki.adj

    echo "----------------------------------------------"    
done

done
echo "===================================================="
for ((a=75000; a<=75000*5; a+=75000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    ./SSSP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    ./SSWP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    ./SSNP -Snapshots 1 -E_number $a ../Twitter/twitter.adj

    echo "----------------------------------------------"    
done

done