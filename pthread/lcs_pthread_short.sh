#!/bin/bash

fname='./test_short.txt'
searchtext='DATA'
#problem=$2
#i=$problem
#'10000 100000 500000 1000000'
#2 is problem size
#3 is thread count
echo $1
#for i in $problem
#do 
	echo "---PROBLEM SIZE: $2---"
	echo "---THREAD COUNT: $3---"
	for ((j = 0; j < 1; j++))
	do
	  ./lcs_pthread $fname $2 $3
	done
#done