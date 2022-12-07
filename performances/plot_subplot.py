#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
This script plots the measures of permoformance of two files into one plot with sublots.
Each suplot have the mean and the standard deviation of the measures.

USAGE:   python3 plot_subplot.py <performance_files_1> <performance_file_2> <output_file> 
EXAMPLE: python3 plot_measures.py performance1.csv performance2.csv performances.pdf
"""
import sys
import matplotlib.pyplot as plt
import pandas as pd

# Reading arguments
file1 = sys.argv[1]
file2 = sys.argv[2]
outfile = sys.argv[3]

# Create dataframes and include mean and std
df1 = pd.read_csv(file1, sep=',')
df2 = pd.read_csv(file2, sep=',')

for df in [df1, df2]:
    df['means'] = df.drop('nbr_threads', axis=1).mean(axis=1)
    df['stds'] = df.drop(['nbr_threads', 'means'], axis=1).std(axis=1)

# Plot
fig = plt.figure()
fig.set_figheight(6)
fig.set_figwidth(10)

fig.text(0.5, 0.04, "Nombre de threads [#]", ha='center')
fig.text(0.04, 0.5, "Temps d'exécution [ms]", va='center', rotation='vertical')

ax = plt.subplot(2, 1, 1)
ax.set_title("Exécution locale (8 coeurs)")
ax.errorbar(df1['nbr_threads'], df1['means']*1000, yerr=df1['stds']*1000, color="C0", ecolor="C0", linestyle='None', marker='.', markeredgewidth=2.10, capsize=5, capthick=1)
ax.grid(True)
plt.xscale('log', base=2)
plt.ylim(-5, 1000*max(df1['means'].max() + df1['stds'].max(), df2['means'].max() + df2['stds'].max())*1.05)

ax = plt.subplot(2, 1, 2)
ax.set_title("Exécution INGInious (32 coeurs)")
ax.errorbar(df2['nbr_threads'], df2['means']*1000, yerr=df2['stds']*1000, color="C1", ecolor="C1", linestyle='None', marker='.', markeredgewidth=2.10, capsize=5, capthick=1)
ax.grid(True)
plt.xscale('log', base=2)
plt.ylim(-5, 1000*max(df1['means'].max() + df1['stds'].max(), df2['means'].max() + df2['stds'].max())*1.05)

plt.savefig(outfile, bbox_inches='tight')
