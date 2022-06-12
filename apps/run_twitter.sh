#!/bin/bash
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 150000 ../Twitter/twitter.adj
done
echo "next batch"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 300000 ../Twitter/twitter.adj
done
echo "next batch"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 450000 ../Twitter/twitter.adj
done
echo "next batch"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 600000 ../Twitter/twitter.adj
done
echo "next batch"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 750000 ../Twitter/twitter.adj
done
echo "next batch"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 900000 ../Twitter/twitter.adj
done
echo "next batch"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 1050000 ../Twitter/twitter.adj
done
echo "next batch"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 1200000 ../Twitter/twitter.adj
done
echo "next batch"
for n in {1..3}; 
do
 ./SSSP -Snapshots 1 -E_number 1350000 ../Twitter/twitter.adj
done
# echo "next batch"
# for n in {1..3}; 
# do
#  ./SSSP -Snapshots 1 -E_number 700000 ../Twitter/twitter.adj
# done