#!/bin/bash


for i in `seq 30`
do
    for j in `seq -100 10 100`
    do
		./graf_pres -o mesh_voltage_data.txt -y $j
	done
done
