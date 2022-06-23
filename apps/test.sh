#!/bin/bash
# make BFS SSSP SSWP SSNP
# # for ((a=70000; a<=70000*20; a+=70000))
# # do
# echo "***********SSWP starts***********"
# for i in {0..5}; do printf '%s\n' {A..C} | paste -sd " " >> file.out; done

    # ./SSWP -Snapshots 1 -E_number 35000 ../LiveJ/LJ.adj >> test.out
    # ./SSWP -Snapshots 1 -E_number 35000 ../LiveJ/LJ.adj >> test.out
    # ./SSWP -Snapshots 1 -E_number 35000 ../LiveJ/LJ.adj >> test.out
    # echo "----------------------------------------------"    

# echo "----------------------------------------------" 
for ((i=0; i<5; i ++))
do
    # printf '%s\n'
    ./SSWP -Snapshots 1 -E_number 70000 ../LiveJ/LJ.adj >> test.out
done
cat test.out | tr '\n' ' ' >> test1.out
for ((i=0; i<5; i ++))
do
    # printf '%s\n'
    ./SSWP -Snapshots 1 -E_number 70000 ../LiveJ/LJ.adj >> test.out
done
cat test.out | tr '\n' ' ' >> test1.out
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
# echo "TT start"
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 100000 ../TT/TT.adj
      
# done
# echo "----------------------------------------------"  
# for ((i=0; i<5; i ++))
# do
#     ./SSWP -Snapshots 1 -E_number 200000 ../TT/TT.adj
        
# done
# echo "----------------------------------------------"
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 100000 ../TT/TT.adj
        
# done
# echo "----------------------------------------------"
# for ((i=0; i<5; i ++))
# do
#     ./SSNP -Snapshots 1 -E_number 200000 ../TT/TT.adj
       
# done
# echo "----------------------------------------------" 
# make BFS SSSP SSWP

# # echo "TT start"
# # for ((a=100000; a<=100000*50; a+=100000))
# # do
# # for ((i=0; i<5; i ++))
# # do
# #     ./BFS -source 100 -Snapshots 1 -E_number $a ../TT/TT.adj
# #     echo "round " $i    
# # done
# # echo "++++++++++++++++++++++++++++++++++++++++++++++"
# # done
# echo "------------------SSSP starts------------------------"
# echo "Live-J start"
# for ((a=35000; a<=35000*50; a+=35000))
# do
# for ((i=0; i<4; i ++))
# do
#     ./SSSP -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
#     echo "round " $i    
# done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
# done

# echo "Wiki start"
# for ((a=40000; a<=40000*50; a+=40000))
# do
# for ((i=0; i<4; i ++))
# do
#     ./SSSP -Snapshots 1 -E_number $a ../wiki/wiki.adj
#     echo "round " $i    
# done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
# done

# echo "Twitter start"
# for ((a=75000; a<=75000*50; a+=75000))
# do
# for ((i=0; i<4; i ++))
# do
#     ./SSSP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
#     echo "round " $i    
# done
# echo "++++++++++++++++++++++++++++++++++++++++++++++"
# done

# # echo "TT start"
# # for ((a=100000; a<=100000*50; a+=100000))
# # do
# # for ((i=0; i<4; i ++))
# # do
# #     ./SSSP -source 100 -Snapshots 1 -E_number $a ../TT/TT.adj
# #     echo "round " $i    
# # done
# # echo "++++++++++++++++++++++++++++++++++++++++++++++"
# # done
# # echo "------------------SSWP starts------------------------"
# # echo "Live-J start"
# # for ((a=35000; a<=35000*50; a+=35000))
# # do
# # for ((i=0; i<4; i ++))
# # do
# #     ./SSWP -Snapshots 1 -E_number $a ../LiveJ/LJ.adj
# #     echo "round " $i    
# # done
# # echo "++++++++++++++++++++++++++++++++++++++++++++++"
# # done

# # echo "Wiki start"
# # for ((a=40000; a<=40000*50; a+=40000))
# # do
# # for ((i=0; i<4; i ++))
# # do
# #     ./SSWP -Snapshots 1 -E_number $a ../wiki/wiki.adj
# #     echo "round " $i    
# # done
# # echo "++++++++++++++++++++++++++++++++++++++++++++++"
# # done

# # echo "Twitter start"
# # for ((a=75000; a<=75000*50; a+=75000))
# # do
# # for ((i=0; i<4; i ++))
# # do
# #     ./SSWP -Snapshots 1 -E_number $a ../Twitter/twitter.adj
# #     echo "round " $i    
# # done
# # echo "++++++++++++++++++++++++++++++++++++++++++++++"
# # done

# # echo "TT start"
# # for ((a=100000; a<=100000*50; a+=100000))
# # do
# # for ((i=0; i<4; i ++))
# # do
# #     ./SSWP -source 100 -Snapshots 1 -E_number $a ../TT/TT.adj
# #     echo "round " $i    
# # done
# # echo "++++++++++++++++++++++++++++++++++++++++++++++"
# # done