#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
TODO: Faire la desciption du programme
"""


import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Read the datas
filename = sys.argv[1]
name = sys.argv[2]

df = pd.read_csv(filename, sep=',')
print(df)

# For each thread number, compute the mean, the max and the min values
df['means'] = df.drop('nbr_threads', axis=1).mean(axis=1)
df['max'] = df.drop('nbr_threads', axis=1).max(axis=1)
df['min'] = df.drop('nbr_threads', axis=1).min(axis=1)

# Plot the results
plt.figure()
plt.plot(df['nbr_threads'], df['means'], label='mean')
plt.errorbar(df['nbr_threads'], df['means'], yerr=[df['means']-df['min'], df['max']-df['means']], fmt='o')
plt.xlabel('Number of threads')
plt.ylabel('Time (s)')
plt.grid(True)

plt.savefig(name + '.pdf')

print(df)