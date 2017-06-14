#!/bin/bash

gcc -o t2 lsteil-t2SO.c -lm -lpthread

for i in $(seq 1 $3);
do
	./t2 $1 $2
	echo "--------------------------"
	sleep 1
done
