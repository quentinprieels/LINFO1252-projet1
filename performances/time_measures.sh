#!/bin/bash
# ================================================================================================================================
# This script will collect the time that the system need to run the program.
# 
# USAGE:    ./collect_time.sh <CSV file to store result> <program to run> <number of times to run (optional, default 5)>
# EXEMPLE:  ./collect_time.sh time_measures.csv ./program 4 10
# OPTIONALS: The fourth argument is the number of times to run the program. Default is 5.
# 
# DESCRIPTION:   This script will run the programm passed in argument a certain number of times (default 5) and will store the time
#                that the system need to run the program in a CSV file. The CSV file will be created if it does not exist.
#                The format of the time is the same as the 'time' command => [hours:]minutes:seconds.
#                The program will be run with 1, 2, 4, 8, 16, 32 and 64 threads.
# ================================================================================================================================

# ==========    Check arguments    ==========
# Check if the number of arguments is correct
if [ $# -lt 2 ] || [ $# -gt 3 ]; then
    echo "Wrong number of arguments. You should pass 2 or 3 arguments."
    echo "USAGE:        ./collect_time.sh <CSV file to store result> <program to run> <number of times to run (optional, default 5)>"
    echo "EXEMPLE:      ./collect_time.sh time_measures.csv ./program 4"
    echo "OPTIONALS:    The third argument is the number of times to run the program. Default is 5."
    exit 1
fi

# Check if the program exist
if [ ! -f $PROGRAM ]; then
    echo "The program $2 doesn't exist."
    exit 1
fi

# ==========    Define variables    ==========
NBR_MEASURES=5
FILE=$1
PROGRAM=$2
THREAD_SEQ=(1 2 4 8 16 32 64)
NBR_THREAD=${#THREAD_SEQ[@]}

# Set the NBR_MEASURES TO $3 if exist
if [ $# -eq 3 ]; then
    NBR_MEASURES=$3
fi

# ==========    Prepare file    ==========
echo -n "nbr_threads" > $FILE
for i in $(seq 1 $NBR_MEASURES); do
    echo -n ",run_${i}" >> $FILE
    if [ $i -eq $NBR_MEASURES ]; then
        echo "" >> $FILE
    fi
done


# ==========    Make measures    ==========
for i in ${THREAD_SEQ[@]}; do
    echo -n $i >> $FILE
    for j in $(seq 1 $NBR_MEASURES); do
        echo -n ",$(/usr/bin/time -f %e $PROGRAM $i $i 2>&1 | grep -v ar)" >> $FILE
    done
    echo "" >> $FILE
done

# ==========    Clear the output    ==========
exit 0