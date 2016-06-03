#!/bin/bash


for i in `seq 30`
do
    for j in `seq 100 -5 0`
    do
			./graf_pres -o ar_co2_data.txt -a $j -c $((100-$j))
	done
done
