#!/bin/bash
make BFS
echo "Live-J start"
for ((a=35000; a<=35000*50; a+=35000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
    echo "round " $i    
done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done

echo "Wiki start"
for ((a=40000; a<=40000*50; a+=40000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../wiki/wiki.adj
    echo "round " $i    
done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done

echo "Twitter start"
for ((a=75000; a<=75000*50; a+=75000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    echo "round " $i    
done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done

echo "TT start"
for ((a=100000; a<=100000*50; a+=100000))
do
for ((i=0; i<5; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../TT/TT.adj
    echo "round " $i    
done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done

