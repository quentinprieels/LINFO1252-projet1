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
import numpy as np

# Global variables
colors = ["C0", "C1", "g", "m", "c", "y"]

filename = sys.argv[1]
output = sys.argv[2]

# Create dataframes and include mean, median and std
df = pd.read_csv(filename, sep=',')
df['means'] = df.drop('nbr_threads', axis=1).mean(axis=1)
df['stds'] = df.drop(['nbr_threads', 'means'], axis=1).std(axis=1)
df['medians'] = df.drop(['nbr_threads', 'means', 'stds'], axis=1).median(axis=1)

# Plot functions
def make_plot(maxi):
    """
    Create a plot with the good size, the good scale and the good axis limits.
    @param maxi : the maximum value of the measures
    """
    fig = plt.figure()
    fig.set_figheight(5)
    fig.set_figwidth(7)
    ax = fig.add_subplot(111)
    ax.grid(True)
    ax.set_xscale('log', base=2)
    plt.ylim(-5, 1000*maxi*1.05)
    return fig, ax    

def error_plot(df, couleur, output):
    """
    Plot the mean and the standard deviation of the measures.
    @param df : the dataframe containing the measures
    @param couleur : the color of the plot
    @param output : the name of the output file
    """
    maximum = df['means'].max() + df['stds'].max()
    fig, ax = make_plot(maximum)
    plt.scatter(df['nbr_threads'], df['medians']*1000, s=15, color=couleur, label = "Médiane")
    plt.errorbar(df['nbr_threads'], df['means']*1000, yerr=df['stds']*1000, color=couleur, ecolor="r", linestyle='None', marker=0, markeredgewidth=2.10, capsize=5, capthick=1, label="Moyenne & Ecart-type")
    fig.tight_layout()
    ax.set_xlabel('Nombre de threads [#]')
    ax.set_ylabel("Temps d'exécution [ms]")
    ax.legend()
    plt.savefig(output, bbox_inches='tight')

# Main
error_plot(df, colors[0], output)