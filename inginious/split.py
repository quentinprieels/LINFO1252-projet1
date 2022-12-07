#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
This script split a csv file into several files.

USAGE:   python3 split.py <input_file> <output_folder>
EXAMPLE: python3 split.py run1.csv measures/

DESCRIPTION:    This script will analyze the input file and split it into several files.
                The output files will be named by the header of the time measure given in the input file.
"""

import sys

# Read the datas
filename = sys.argv[1]
folder = sys.argv[2]

with open(filename, 'r') as input_file:
    time_file = input_file.read()
    time_file = time_file.split("\n\n")
    
    for i in range(len(time_file)):
        header = time_file[i].split("\n")[0]
        
        # remove only the first line
        measure = time_file[i][len(header)+1:]

        with open(folder + header, 'w+') as output_file:
            output_file.write(measure)
