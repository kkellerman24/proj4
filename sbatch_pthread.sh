#!/bin/bash

#	10 runs each
#	Type(3): pthreads, openmp, mpi
#	Cpu(10): 1,2,4,8,16,32,2x4, 4x4, 16x2, 2x16
#	Problem size(4): 10k, 100k, 500k, 1M
mem='32G'
cons='elves'
ptype='pthreads'
capptype='PTHREADS'
nthreads='1 2 4 8 16 32'
problem='10000 100000 500000 1000000'

ncores='1 2 4 8 16'

#cores
for i in $ncores
do
  for j in $problem
  do
	#1 run
    for k in $nthreads
    do 
#     for ((a = 0; a < 1; a++))
#      do
        sbatch --constraint=$cons --mail-type=END --time=1:00:00 --mem=$mem --cpus-per-task=$i lcs_$ptype.sh "$capptype $cons CORES: $i" $j $k
#      done 
   done
  done
done