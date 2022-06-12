#!/bin/bash
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 70000 ../LiveJ/LJ.adj
done
echo "next batch"
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 140000 ../LiveJ/LJ.adj
done
echo "next batch"
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 210000 ../LiveJ/LJ.adj
done
echo "next batch"
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 280000 ../LiveJ/LJ.adj
done
echo "next batch"
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 350000 ../LiveJ/LJ.adj
done
echo "next batch"
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 420000 ../LiveJ/LJ.adj
done
echo "next batch"
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 490000 ../LiveJ/LJ.adj
done
echo "next batch"
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 560000 ../LiveJ/LJ.adj
done
echo "next batch"
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 630000 ../LiveJ/LJ.adj
done
echo "next batch"
for n in {1..10}; 
do
 ./SSSP -Snapshots 1 -E_number 700000 ../LiveJ/LJ.adj
done