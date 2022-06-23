#!/bin/bash
# make BFS SSSP SSWP SSNP
# # for ((a=70000; a<=70000*20; a+=70000))
# # do
# echo "***********SSWP starts***********"
# echo "LJ start"
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 35000 ../LiveJ/LJ.adj
#     # echo "----------------------------------------------"    
# done
# echo "----------------------------------------------" 
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 70000 ../LiveJ/LJ.adj
# done
#     echo "----------------------------------------------"    

# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 35000 ../LiveJ/LJ.adj
      
# done
# echo "----------------------------------------------"  
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 70000 ../LiveJ/LJ.adj
       
# done
# echo "----------------------------------------------" 
# echo "Wiki start"
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 40000 ../wiki/wiki.adj
        
# done
# echo "----------------------------------------------"
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 80000 ../wiki/wiki.adj
       
# done
# echo "----------------------------------------------" 
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 40000 ../wiki/wiki.adj
       
# done
# echo "----------------------------------------------" 
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 80000 ../wiki/wiki.adj
       
# done
# echo "----------------------------------------------" 
# echo "Twitter start"
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 75000 ../Twitter/twitter.adj
      
# done
# echo "----------------------------------------------"  
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 150000 ../Twitter/twitter.adj
       
# done
# echo "----------------------------------------------" 
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 75000 ../Twitter/twitter.adj
    
# done
# echo "----------------------------------------------"    
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 150000 ../Twitter/twitter.adj
       
# done
# echo "----------------------------------------------" 
echo "TT start"
for ((i=0; i<5; i ++))
do
    ./SSWP -source 100 -Snapshots 1 -E_number 100000 ../TT/TT.adj
      
done
echo "----------------------------------------------"  
for ((i=0; i<5; i ++))
do
    ./SSWP -Snapshots 1 -E_number 200000 ../TT/TT.adj
        
done
echo "----------------------------------------------"
for ((i=0; i<5; i ++))
do
    ./SSNP -Snapshots 1 -E_number 100000 ../TT/TT.adj
        
done
echo "----------------------------------------------"
for ((i=0; i<5; i ++))
do
    ./SSNP -Snapshots 1 -E_number 200000 ../TT/TT.adj
       
done
echo "----------------------------------------------" 
