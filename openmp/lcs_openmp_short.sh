#!/bin/bash

fname='./test_short.txt'
searchtext='DATA'
#2 is problem size
#3 is thread count
echo $1
#for i in $problem
#do 
	echo "---PROBLEM SIZE: $2---"
	echo "---THREAD COUNT: $3---"
	for ((j = 0; j < 10; j++))
	do
	  ./lcs_openmp $fname $2 $3 | grep $searchtext
	done
#done
