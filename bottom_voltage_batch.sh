#!/bin/bash


for i in `seq 30`
do
    for j in `seq 300 25 800`
    do
		./graf_pres -o bottom_voltage_data.txt -z $j
	done
done
