#!/bin/bash

#	1 run
#	MPI
#	Cpu(10): 1,2,4,8,16,32,2x4, 4x4, 16x2, 2x16
#	Problem size: 1k, 10k, 100k, 500k, 1M
mem='32G'
cons='elves'
ptype='mpi'
capptype='MPI'
#nthreads='1 2 4 8 16 32'
problem='1000 10000 100000 500000 1000000'
ncores='1 2 4 8 16'

#cores
for i in $ncores
do
  for j in $problem
  do
	sbatch --constraint=$cons --mail-type=END --time=24:00:00 --mem=$mem --ntasks-per-node=$i lcs_$ptype.sh "$capptype $cons CORES: $i" $j $i
  done
done
