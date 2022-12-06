#!/bin/bash
# ================================================================================================================================
# This script is launched by the Inginious platform to run the program and collect the time that the system need to run the 
# program.
#
# USAGE:    ./collect_time.sh
# ================================================================================================================================

# Compile the program
make compile -s

# Collect the time for task 1
./performances/time_measures.sh philosophes_ingi.csv bins/philosophes
./performances/time_measures.sh producer_consumer_ingi.csv bins/producer_consumer
./performances/time_measures.sh reader_writer_ingi.csv bins/reader_writer

# Print CSV files in the 
echo "philosophes_ingi.csv"
cat performances/philosophes_ingi.csv
echo ""

echo "producer_consumer_ingi.csv"
cat performances/producer_consumer_ingi.csv
echo ""

echo "reader_writer_ingi.csv"
cat performances/reader_writer_ingi.csv
echo ""

# Collect the time for task 2
./performances/time_measures.sh test_and_set_ingi.csv bins/test_and_set
./performances/time_measures.sh test_and_test_and_set_ingi.csv bins/test_and_test_and_set

# Print CSV files in the
echo "test_and_set.csv"
cat performances/test_and_set.csv
echo ""

echo "test_and_test_and_set.csv"
cat performances/test_and_test_and_set.csv
echo ""


# Clean all files
make clean -s
