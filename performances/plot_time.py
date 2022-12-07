import pandas as pd
import sys
import matplotlib.pyplot as plt

file1 = sys.argv[1]
file2 = sys.argv[2]
outfile = sys.argv[3]

fil1_name = file1.split("/")[-1].replace("_ingi.csv", "")
fil2_name = file2.split("/")[-1].replace("_ingi.csv", "")

df1 = pd.read_csv(file1, sep=',')
df2 = pd.read_csv(file2, sep=',')

for df in [df1, df2]:
    df['means'] = df.drop('nbr_threads', axis=1).mean(axis=1)
    df['stds'] = df.drop(['nbr_threads', 'means'], axis=1).std(axis=1)

fig = plt.figure()
fig.set_figheight(5)
fig.set_figwidth(7)
plt.errorbar(df1['nbr_threads'], df1['means']*1000, yerr=df1['stds']*1000, color="C0", ecolor="C0", linestyle='None', marker='.', markeredgewidth=2.10, capsize=5, capthick=1)
plt.errorbar(df2['nbr_threads'], df2['means']*1000, yerr=df2['stds']*1000, color="C1", ecolor="C1", linestyle='None', marker='.', markeredgewidth=2.10, capsize=5, capthick=1)
plt.plot(df1['nbr_threads'], df1['means']*1000, color="C0", linestyle='--', label=fil1_name)
plt.plot(df2['nbr_threads'], df2['means']*1000, color="C1", linestyle='--', label=fil2_name)
plt.grid(True)
plt.xscale('log', base=2)
plt.ylim(-5, 1000*max(df1['means'].max() + df1['stds'].max(), df2['means'].max() + df2['stds'].max())*1.05)
plt.xlabel('Nombre de threads [#]')
plt.ylabel('Temps d\'exécution [ms]')
plt.legend()
plt.savefig(outfile, bbox_inches='tight')
