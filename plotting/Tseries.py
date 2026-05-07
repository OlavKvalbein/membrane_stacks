import numpy as np
import matplotlib.pyplot as plt

from timeseries import read_timeseries_folder

# folderpath = "data/timeseries/L=32,Lz=8/J=0.1"
# folderpath = "data/timeseries/L=32,Lz=8/J=0.5"
folderpath = "data/timeseries/L=32,Lz=8/J=1.0"

n_sites = 32*32*8
# n_sites = 16*16*8
Ts, steps, Hs, delta2s = read_timeseries_folder(folderpath)

avg_Hs = []
std_Hs = []
avg_delta2s = []

for i in range(len(Ts)):
    avg_Hs.append(Hs[i].mean() / (n_sites))
    std_Hs.append(Hs[i].std(ddof=0))
    avg_delta2s.append(delta2s[i].mean())

plt.figure(1)
# ax[0].errorbar(Ts, avg_Hs, yerr=std_Hs, fmt="o")
plt.scatter(Ts, avg_Hs)
plt.title("Mean Energy")
plt.grid()
plt.xlabel(r"$T \,/\, J$")
plt.ylabel(r"$\langle H \rangle \,/\, N$")
plt.savefig(folderpath + "/energy.png")

plt.figure(2)
plt.scatter(Ts, avg_delta2s)
plt.title("Layer Interconnectivity")
plt.ylabel(r"$\delta^2$")
plt.xlabel(r"$T \,/\, J$")
plt.grid()
plt.savefig(folderpath + "/delta2.png")