#!/bin/bash
make BFS SSSP SSWP SSNP
echo "BFS start"
# for ((a=85000; a<=85000*50; a+=85000))
# do
for ((i=0; i<5; i ++))
do
   ./BFS -source 10000 -Snapshots 1 -E_number 85000 ../dl/dl.adj 
done
echo "----------------------------------------------"
# done
echo "SSSP start"
# for ((a=85000; a<=85000*50; a+=85000))
# do
for ((i=0; i<5; i ++))
do
    ./SSSP -source 10000 -Snapshots 1 -E_number 85000 ../dl/dl.adj 
done
echo "----------------------------------------------"
# done
echo "SSWP start"
# for ((a=85000; a<=85000*50; a+=85000))
# do
for ((i=0; i<5; i ++))
do
    ./SSWP -source 10000 -Snapshots 1 -E_number 85000 ../dl/dl.adj
done
echo "----------------------------------------------"
# done
echo "SSNP start"
# for ((a=85000; a<=85000*50; a+=85000))
# do
for ((i=0; i<5; i ++))
do
    ./SSNP -source 10000 -Snapshots 1 -E_number 85000 ../dl/dl.adj
done
echo "----------------------------------------------"
# done