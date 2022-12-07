#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
This script plots the measures of the performances of a model.

USAGE:   python3 plot_measures.py <list_of_performance_files> <output_file> <optionnal : type>
EXAMPLE: python3 plot_measures.py performance1.csv,performance2.csv,performance3.csv performances.pdf same

PRE-CONDITIONS: performance_files take the extension .csv and are separated by a "," without any space between
                type can be "same" or "different", by default or if non-specified : type="different"
DESCRIPTION:    This script will analyze the performance file(s) and plot the mean an the standard deviation of the measures.
                To generate the performance file, use the script performances/timeseries_measures.sh
                The output file will be a file containing the plot(s) (the extentions type must be given).
"""
import sys
import pandas as pd
import matplotlib.pyplot as plt

# Read the datas
filenames = sys.argv[1].split(',') # liste de nom de fichiers
n = len(filenames)
name = sys.argv[2] # nom du fichier d'output
try :
    type = sys.argv[3] # "same" or "different"
except :
    type = "different"

dfs = []
for filename in filenames:
    dfs.append(pd.read_csv(filename, sep=','))

# For each thread number, compute the mean, the standard deviation
for df in dfs:
    df['means'] = df.drop('nbr_threads', axis=1).mean(axis=1)
    df['stds'] = df.drop('nbr_threads', axis=1).std(axis=1)

# Plot the results
# Create figure
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)


if type == "same":
    for i in range(n):
        plt.errorbar(dfs[i]['nbr_threads'], dfs[i]['means']*1000, yerr=dfs[i]['stds']*1000, ecolor='red', linestyle='None', marker='.', markeredgewidth=2.5, capsize=5, capthick=1)
        ax.set_xscale('log', base=2)
        ax.set_ylim(bottom=-5)  
        plt.grid(True)
else :
    # On change la taille de la figure pour que ce soit plus joli + on cache le gros plot qui contient tous les subplots
    fig.set_figwidth(n*5)
    fig.set_figheight(5)
    ax.spines['top'].set_color('none')
    ax.spines['bottom'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.spines['right'].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)

    # rajout des subplots à l'horizontal
    for i in range(n):
        axi = fig.add_subplot(1, n, i+1)
        axi.errorbar(dfs[i]['nbr_threads'], dfs[i]['means']*1000, yerr=dfs[i]['stds']*1000, ecolor='red', linestyle='None', marker='.', markeredgewidth=2.5, capsize=5, capthick=1)
        axi.set_xscale('log', base=2)
        axi.set_ylim(bottom=0)
        axi.grid(True)
        axi.set_title(filenames[i].split('/')[-1].rstrip('.csv'))

# Plot parameters
fig.tight_layout()
ax.set_xlabel('Number of threads [#]')
ax.set_ylabel('Time [ms]')
plt.savefig(name, bbox_inches='tight')
