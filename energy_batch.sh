#!/bin/bash


for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 0.1e9
done

for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 1e9
done

for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 10e9
done

for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 25e9
done

for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 50e9
done
