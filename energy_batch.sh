#!/bin/bash


for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 0.1.e9
done

for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 1.e9
done

for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 10.e9
done

for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 25.e9
done

for i in `seq 250`
do
	./graf_pres -o energy_data.txt -e 50.e9
done
