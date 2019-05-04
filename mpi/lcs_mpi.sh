#!/bin/bash

fname='/homes/dan/625/wiki_dump.txt'
searchtext='DATA'
nthreads=$3
module load OpenMPI
echo $1
echo "---PROBLEM SIZE: $2---"
echo "---THREAD COUNT: $3---"
for ((j = 0; j < 1; j++))
do
	mpirun -np $nthreads lcs_mpi $fname $2
done