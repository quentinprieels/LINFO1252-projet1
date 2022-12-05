#!/bin/bash
# ================================================================================================================================
# This script is launched by the Inginious platform to run the program and collect the time that the system need to run the 
# program.
#
# USAGE:    ./collect_time.sh
# ================================================================================================================================

# Compile the program
make compile -s

# Collect the time
./performances/time_measures.sh philosophes_ingi.csv bins/philosophes
./performances/time_measures.sh producer_consumer_ingi.csv bins/producer_consumer
# ./performances/time_measures.sh reader_writer_ingi.csv bins/reader_writer

# Print CSV files in the 
echo "philosophes_ingi.csv"
cat performances/philosophes_ingi.csv
echo ""
echo "producer_consumer_ingi.csv"
cat performances/producer_consumer_ingi.csv
# echo "reader_writer_ingi.csv"
# cat performances/reader_writer_ingi.csv

# Clean all files
make clean -s
