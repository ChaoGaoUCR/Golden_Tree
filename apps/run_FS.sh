#!/bin/bash
make SSSP
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 125000 ../FS/FS.adj
done
echo "next batch"
echo "+++++++++++"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 250000 ../FS/FS.adj
done
echo "next batch"
echo "+++++++++++"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 375000 ../FS/FS.adj
done
echo "next batch"
echo "+++++++++++"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 500000 ../FS/FS.adj
done
echo "next batch"
echo "+++++++++++"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 625000 ../FS/FS.adj
done
echo "next batch"
echo "+++++++++++"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 750000 ../FS/FS.adj
done
echo "next batch"
echo "+++++++++++"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 875000 ../FS/FS.adj
done
echo "next batch"
echo "+++++++++++"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 1000000 ../FS/FS.adj
done
echo "next batch"
echo "+++++++++++"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 1125000 ../FS/FS.adj
done
echo "next batch"
echo "+++++++++++"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 1250000 ../FS/FS.adj
done