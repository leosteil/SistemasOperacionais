#!/bin/bash

gcc -o t1  lsteil-t1SO.c
for i in $(seq 1 30)
do
	echo -------------------------------------
	./t1 $1
done
