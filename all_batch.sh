#!/bin/bash

./energy_batch.sh &> ./energy_batch.log &
./ar_co2_batch.sh &> ./ar_co2_batch.log &
./top_voltage_batch.sh &> ./top_voltage_batch.log &
./bottom_voltage_batch.sh &> ./bottom_voltage_batch.log &
./mesh_voltage_batch.sh &> ./mesh_voltage_batch.log &
./pressure_batch.sh &> ./pressure_batch.log &
./temperature_batch.sh &> ./temperature_batch.log &

