#!/bin/bash

#	1 run
#	Type(3): pthreads, openmp, mpi
#	Cpu(10): 1,2,4,8,16,32,2x4, 4x4, 16x2, 2x16
#	Problem size(5): 1k, 10k, 100k, 500k, 1M
mem='32G'
cons='elves'
ptype='mpi'
capptype='MPI'
problem='1000 10000 100000 500000 1000000'

#CPUs x Cores
#2x4, 4x4, 16x2, 2x16
for j in $problem
do
 sbatch --constraint=$cons --mail-type=END --time=24:00:00 --mem=$mem --nodes=4 lcs_$ptype.sh "$capptype $cons CONFIG: 2x4" $j 2
done

for j in $problem
do
 sbatch --constraint=$cons --mail-type=END --time=24:00:00 --mem=$mem --nodes=4 lcs_$ptype.sh "$capptype $cons CONFIG: 4x4" $j 4
done

for j in $problem
do
  sbatch --constraint=$cons --mail-type=END --time=24:00:00 --mem=$mem --nodes=2 lcs_$ptype.sh "$capptype $cons CONFIG: 16x2" $j 16
done

for j in $problem
do 
  sbatch --constraint=$cons --mail-type=END --time=24:00:00 --mem=$mem --nodes=16 lcs_$ptype.sh "$capptype $cons CONFIG: 2x16" $j 2
done
