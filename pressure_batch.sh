#!/bin/bash



for i in `seq 30`
do
    for j in `seq 110 50 2260`
    do
		./graf_pres -o pressure_data.txt -p $j
	done
done
