#!/bin/bash

gcc -o t1 lsteil-t1SO.c

for i in $(seq 1 $2);
do
	./t1 $1 $2
	echo "--------------------------"
done
