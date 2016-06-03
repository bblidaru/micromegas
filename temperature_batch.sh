#!/bin/bash


for i in `seq 30`
do
    for j in `seq 98 25 448`
    do
		./graf_pres -o temperature_data.txt -t $j
	done
done
