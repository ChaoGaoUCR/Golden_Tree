echo "BFS start"
for ((a=75000; a<=75000*50; a+=75000))
do
for ((i=0; i<5; i ++))
do
    ./BFS -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    # echo "round " $i    
done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done

echo "SSSP start"
for ((a=75000; a<=75000*50; a+=75000))
do
for ((i=0; i<5; i ++))
do
    ./SSSP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    # echo "round " $i    
done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done

echo "SSWP start"
for ((a=75000; a<=75000*50; a+=75000))
do
for ((i=0; i<5; i ++))
do
    ./SSWP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    # echo "round " $i    
done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done

echo "SSNP start"
for ((a=75000; a<=75000*50; a+=75000))
do
for ((i=0; i<5; i ++))
do
    ./SSNP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
    # echo "round " $i    
done
echo "++++++++++++++++++++++++++++++++++++++++++++++"
done