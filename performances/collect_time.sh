#!/usr/bin/bash
# This script will collect the time that the system need to run the program

FILE=time_mesures.csv

echo "Number of threads,run 1,run 2,run 3,run 4,run 5"

# Make mesures
for i in 1 2 3 4 5 6 7 8
do
    echo -n $i
    for j in 1 2 3 4 5
    do
        echo -n ",$(/usr/bin/time -f %e make -j $i -s 2>&1 | grep -v ar)"
        make -s clean
    done
    echo 
done