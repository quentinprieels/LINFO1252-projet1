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

def make_plot(maxi):
    fig = plt.figure()
    fig.set_figheight(5)
    fig.set_figwidth(7)
    ax = fig.add_subplot(111)
    ax.grid(True)
    ax.set_xscale('log', base=2)
    plt.ylim(-5, 1000*maxi*1.05)
    return fig, ax    

def error_plot(df, couleur, name):
    maximum = df['means'].max() + df['stds'].max()
    fig, ax = make_plot(maximum)
    plt.scatter(df['nbr_threads'], df['medians']*1000, s=15, color=couleur, label = "Médiane")
    plt.errorbar(df['nbr_threads'], df['means']*1000, yerr=df['stds']*1000, color=couleur, ecolor="r", linestyle='None', marker=0, markeredgewidth=2.10, capsize=5, capthick=1, label="Moyenne & Ecart-type")
    fig.tight_layout()
    ax.set_xlabel('Nombre de threads [#]')
    ax.set_ylabel('Temps [ms]')
    ax.legend()
    plt.savefig(name, bbox_inches='tight')

def line_plot(dfs, couleurs, name, filenames, n):
    maximum = np.max([(df['means'].max() + df['stds'].max()) for df in dfs])
    fig, ax = make_plot(maximum)
    for i in range(n):
        plt.scatter(dfs[i]['nbr_threads'], dfs[i]['medians']*1000, s=15, color=couleurs[i%6], label="Médiane de " + filenames[i].split("/")[-1].replace("_ingi.csv", ""))
        plt.plot(dfs[i]['nbr_threads'], dfs[i]['medians']*1000, color=couleurs[i%6])
    fig.tight_layout()
    ax.set_xlabel('Nombre de threads [#]')
    ax.set_ylabel('Temps [ms]')
    ax.legend()
    plt.savefig(name, bbox_inches='tight')

# Read the datas
filenames = sys.argv[1].split(',') # liste de nom de fichiers
n = len(filenames)
name = sys.argv[2] # nom du fichier d'output
try :
    type = sys.argv[3] # "same" or "different" or "line"
except :
    type = "different"

dfs = [pd.read_csv(filename, sep=',') for filename in filenames]
# For each thread number, compute the mean, the standard deviation
for df in dfs:
    df['means'] = df.drop('nbr_threads', axis=1).mean(axis=1)
    df['stds'] = df.drop('nbr_threads', axis=1).std(axis=1)
    df['medians'] = df.drop('nbr_threads', axis=1).median(axis=1)

# Plot the results
# Create figure

colors = ["C0", "C1", "g", "m", "c", "y"]

if type == "multiple":
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    maximum = np.max([(df['means'].max() + df['stds'].max()) for df in dfs])
    plt.ylim(-5,maximum*1.05*1000)
     # On change la taille de la figure pour que ce soit plus joli + on cache le gros plot qui contient tous les subplots
    fig.set_figwidth(n*7)
    for locate in ['top', 'bottom', 'left', 'right']:
        ax.spines[locate].set_color('none')
    ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)   
    # rajout des subplots à l'horizontal
    for i in range(n):
        axi = fig.add_subplot(1, n, i+1)
        axi.errorbar(dfs[i]['nbr_threads'], dfs[i]['means']*1000, yerr=dfs[i]['stds']*1000, color=colors[i%6], ecolor=colors[i%6], linestyle='None', marker=0, markeredgewidth=1.5, capsize=5, capthick=1)
        axi.scatter(dfs[i]['nbr_threads'], dfs[i]['medians']*1000, s=15, color=colors[i%6])
        axi.set_xscale('log', base=2)
        axi.set_ylim(bottom=-5)
        axi.grid(True)
else:
    for i in range(n):
        error_plot(dfs[i], colors[i%6], filenames[i].rstrip(".csv") + ".pdf")
    if type == "line":
        line_plot(dfs, colors, name, filenames, n)