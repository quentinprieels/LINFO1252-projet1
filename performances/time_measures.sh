#! /bin/bash
# ================================================================================================================================
# This script will collect the time that the system need to run the program.
# 
# USAGE:    ./collect_time.sh <CSV file to store result> <program to run> <number of maximal thread> 
# EXEMPLE:  ./collect_time.sh time_measures.csv ./program 4
# OPTIONALS: The third argument is the number of maximal thread to use. Default is given by 2 times the 'nproc' command.
# 
# DESCRIPTION:   This script will run the programm passed in argument with the number of thread passed in argument.
#                If the programm needs two arguments, the second number of thread is passed, if not, this arguments is ignored.
#                The result will be stored in a CSV file. 
#                The format of the time is the same as the 'time' command => [hours:]minutes:seconds.
# ================================================================================================================================

NBR_MEASURES=5
NBR_MIN_THREAD=2  # TODO: Normaly, it should be 1, but the program is 'philosophes' need a least 2 threads to run.

FILE=$1
PROGRAM=$2

# ==========    Check arguments    ==========
# Check if the number of arguments is correct
if [ $# -lt 2 ] || [ $# -gt 3 ]; then
    echo "Wrong number of arguments. You should pass 2 or 3 arguments."
    echo "USAGE:    ./collect_time.sh <CSV file to store result> <program to run> <number of maximal thread>"
    echo "EXEMPLE:  ./collect_time.sh time_measures.csv ./program 4"
    echo "OPTIONALS: The third argument is the number of maximal thread to use. Default is given by the 'nproc' command."
    exit 1
fi  

# Check for the number of maximal thread
if [ $# -eq 3 ]; then
    NBR_THREAD=$3
else
    # Number of thread is two times the number of cores
    NBR_THREAD=$(($(nproc) * 2))
fi
echo "Number of maximal thread used: $NBR_THREAD"


# Check if the program exist
if [ ! -f $PROGRAM ]; then
    echo "The program $2 doesn't exist."
    exit 1
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
for i in $(seq $NBR_MIN_THREAD $NBR_THREAD); do
    echo -n $i >> $FILE
    for j in $(seq 1 $NBR_MEASURES); do
        echo -n ",$(/usr/bin/time -f %e $PROGRAM $i $i 2>&1 | grep -v ar)" >> $FILE
    done
    echo "" >> $FILE
done

# ==========    Clear the output    ==========
mv $FILE performances/$FILE
exit 0