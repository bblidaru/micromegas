#!/bin/bash


for i in `seq 20`
do
    for j in `seq 120 25 400`
	./graf_pres -o temperature_data.txt -t $j
done
