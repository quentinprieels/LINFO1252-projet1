#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
This script plots the measures of the performances of a model.

USAGE:   python3 plot_measures.py <performance_file> <output_file>
EXAMPLE: python3 plot_measures.py performance.csv performance.pdf

DESCRIPTION:    This script will analyze the performance file and plot the mean an the standard deviation of the measures.
                To generate the performance file, use the script performances/timeseries_measures.sh
                The output file will be a file containing the plot (the extentions type must be given).
"""
import sys
import os
import pandas as pd
import matplotlib.pyplot as plt

# Read the datas
filename = sys.argv[1]
name = sys.argv[2]

df = pd.read_csv(filename, sep=',')

# For each thread number, compute the mean, the standard deviation
df['means'] = df.drop('nbr_threads', axis=1).mean(axis=1)
df['stds'] = df.drop('nbr_threads', axis=1).std(axis=1)

# Plot the results
plt.figure()
plt.errorbar(df['nbr_threads'], df['means']*1000, yerr=df['stds']*1000, ecolor='red', linestyle='None', marker='.', markeredgewidth=2.5, capsize=5, capthick=1,)
plt.xlabel('Number of threads')
plt.ylabel('Time [ms]')
plt.grid(True)
plt.savefig(name)

# Move the file to the performances folder
os.system('mv ' + name + ' performances/')
