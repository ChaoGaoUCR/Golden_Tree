#!/bin/bash
make BFS SSSP SSWP SSNP
# # for ((a=70000; a<=70000*20; a+=70000))
# # do
# echo "***********SSWP starts***********"
# echo "LJ start"
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 1750000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 2000000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 1750000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 2000000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
# echo "Wiki start"
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 2000000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 20000000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 2000000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 20000000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
echo "Wiki start"
# for ((i=0; i<5; i ++))
# do
#     ./BFS -Snapshots 1 -E_number 3750000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
for ((i=0; i<5; i ++))
do
    ./BFS -Snapshots 1 -E_number 2000000 ../wiki/wiki.adj
    echo "----------------------------------------------"    
done
echo "----------------------------------------------"    
# for ((i=0; i<5; i ++))
# do
#     ./SSSP -Snapshots 1 -E_number 3750000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
for ((i=0; i<5; i ++))
do
    ./SSSP -Snapshots 1 -E_number 2000000 ../wiki/wiki.adj
    echo "----------------------------------------------"    
done
echo "----------------------------------------------"    
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 3750000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
for ((i=0; i<5; i ++))
do
    ./SSWP -Snapshots 1 -E_number 2000000 ../wiki/wiki.adj
    echo "----------------------------------------------"    
done
echo "----------------------------------------------"    
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 3750000 ../wiki/wiki.adj
#     echo "----------------------------------------------"    
# done
for ((i=0; i<5; i ++))
do
    ./SSNP -Snapshots 1 -E_number 2000000 ../wiki/wiki.adj
    echo "----------------------------------------------"    
done
# echo "TT start"
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -source 100 -Snapshots 1 -E_number 5000000 ../TT/TT.adj
#     echo "----------------------------------------------"    
# done
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -source 100 -Snapshots 1 -E_number 10000000 ../TT/TT.adj
#     echo "----------------------------------------------"    
# done
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -source 100 -Snapshots 1 -E_number 5000000 ../TT/TT.adj
#     echo "----------------------------------------------"    
# done
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -source 100 -Snapshots 1 -E_number 10000000 ../TT/TT.adj
#     echo "----------------------------------------------"    
# done
# make BFS SSSP SSWP
