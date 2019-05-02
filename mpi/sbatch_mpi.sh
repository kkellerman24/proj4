#!/bin/bash

#	1 run
#	Type(3): pthreads, openmp, mpi
#	Cpu(10): 1,2,4,8,16,32,2x4, 4x4, 16x2, 2x16
#	Problem size(4): 10k, 100k, 500k, 1M
mem='4G'
cons='elves'
ptype='mpi'
capptype='MPI'
nthreads='1 2 4 8 16 32'
problem='10000'
ncores='1 2 4 8 16'

for i in $ncores
do
    for j in $problem
    do
        for k in $nthreads
        do
			sbatch --constraint=$cons --mail-type=END --time=1:00:00 --mem=$mem --ntasks-per-node=$i --nodes=1 lcs_$ptype.sh "$capptype $cons CORES: $i" $j $k
       done
    done
done