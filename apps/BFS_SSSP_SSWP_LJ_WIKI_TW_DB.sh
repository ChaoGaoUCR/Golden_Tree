#!/bin/bash
make SSSP BFS SSWP
echo "SSSP now starts"
echo "/***********************************************/"
echo "Live-J start"
for ((a=35000; a<=35000*50; a+=35000))
do
for ((i=0; i<3; i ++))
do
    ./SSSP -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
    echo "round " $i 
    echo "****************************************"   
done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "Wiki start"
for ((a=40000; a<=40000*50; a+=40000))
do
for ((i=0; i<3; i ++))
do
    ./SSSP -Snapshots 1 -E_number $a ../wiki/wiki.adj
    echo "round " $i    
    echo "****************************************"   

done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "Twitter start"
for ((a=75000; a<=75000*50; a+=75000))
do
for ((i=0; i<3; i ++))
do
    ./SSSP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    echo "round " $i    
    echo "****************************************"   

done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "TT start"
for ((a=100000; a<=100000*50; a+=100000))
do
for ((i=0; i<3; i ++))
do
    ./SSSP -source 100 -Snapshots 1 -E_number $a ../TT/TT.adj
    echo "round " $i    
    echo "****************************************"   

done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done

echo "/***********************************************/"
echo "BFS starts"
echo "Live-J start"
for ((a=35000; a<=35000*50; a+=35000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
    echo "round " $i 
    echo "****************************************"   
done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "Wiki start"
for ((a=40000; a<=40000*50; a+=40000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../wiki/wiki.adj
    echo "round " $i    
    echo "****************************************"   

done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "Twitter start"
for ((a=75000; a<=75000*50; a+=75000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    echo "round " $i    
    echo "****************************************"   

done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "TT start"
for ((a=100000; a<=100000*50; a+=100000))
do
for ((i=0; i<3; i ++))
do
    ./BFS -source 100 -Snapshots 1 -E_number $a ../TT/TT.adj
    echo "round " $i    
    echo "****************************************"   

done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done

echo "/***********************************************/"
echo "SSWP starts"
echo "Live-J start"
for ((a=35000; a<=35000*50; a+=35000))
do
for ((i=0; i<3; i ++))
do
    ./SSWP -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
    echo "round " $i 
    echo "****************************************"   
done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "Wiki start"
for ((a=40000; a<=40000*50; a+=40000))
do
for ((i=0; i<3; i ++))
do
    ./SSWP -Snapshots 1 -E_number $a ../wiki/wiki.adj
    echo "round " $i    
    echo "****************************************"   

done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "Twitter start"
for ((a=75000; a<=75000*50; a+=75000))
do
for ((i=0; i<3; i ++))
do
    ./SSWP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    echo "round " $i    
    echo "****************************************"   

done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
done
echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

echo "TT start"
for ((a=100000; a<=100000*50; a+=100000))
do
for ((i=0; i<3; i ++))
do
    ./SSWP -source 100 -Snapshots 1 -E_number $a ../TT/TT.adj
    echo "round " $i    
    echo "****************************************"   

done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done