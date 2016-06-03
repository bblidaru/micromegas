#!/bin/bash


for i in `seq 30`
do
    for j in `seq -25 -10 -295`
    do
		./graf_pres -o top_voltage_data.txt -x $j
	done
done
