#! /bin/bash
# ==============================================================================
# This script will collect the time that the system need to run the program.
# 
# USAGE:    ./collect_time.sh <CSV file to store result> <program to run> <number of maximal thread> 
# EXEMPLE:  ./collect_time.sh time_mesures.csv ./program 4
# 
# DESCRIPTION:   This script will run the programm passed in argument with the number of thread passed in argument.
#                If the programm needs two arguments, the second number of thread is passed, if not, this arguments is ignored.
#                The result will be stored in a CSV file. 
# ==============================================================================

NBR_MEASURES=5

# ==========    Check arguments    ==========
# Check if the number of arguments is correct
if [ $# -ne 3 ]; then
    echo "Wrong number of arguments"
    echo "USAGE:    ./collect_time.sh <CSV file to store result> <program to run> <number of maximal thread 1>"
    echo "EXEMPLE:  ./collect_time.sh time_mesures.csv ./program 4"
    exit 1
fi


# Check if the program exist
if [ ! -f $2 ]; then
    echo "The program $2 doesn't exist."
    exit 1
fi

# ==========    Prepare file    ==========
echo -n "Number of threads" > $1
for i in $(seq 1 $NBR_MEASURES); do
    echo -n ",run ${i}" >> $1
    if [ $i -eq $NBR_MEASURES ]; then
        echo "" >> $1
    fi
done
