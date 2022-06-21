#!/bin/bash
make SSSP
echo "Live-J start"
for ((a=35000; a<=35000*2; a+=35000))
do
for ((i=0; i<5; i ++))
do
    ./SSSP -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
    echo "round " $i 
    echo "****************************************"   
done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
echo "Twitter start"
for ((a=75000; a<=75000*2; a+=75000))
do
for ((i=0; i<5; i ++))
do
    ./SSSP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    echo "round " $i    
    echo "****************************************"   

done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

# echo "FS start"
# for ((a=100000; a<=150000; a+=50000))
# do
# for ((i=0; i<5; i ++))
# do
#     ./SSSP -Snapshots 1 -E_number $a ../FS/FS.adj
#     echo "round " $i    
    # echo "****************************************"   

# done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
# done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "Wiki start"
for ((a=40000; a<=80000; a+=40000))
do
for ((i=0; i<5; i ++))
do
    ./SSSP -Snapshots 1 -E_number $a ../wiki/wiki.adj
    echo "round " $i    
    echo "****************************************"   

done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done