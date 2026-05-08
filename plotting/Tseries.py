import numpy as np
import matplotlib.pyplot as plt

from timeseries import read_timeseries_folder

# folderpath = "data/timeseries/L=32,Lz=8/J=0.1"
# folderpath = "data/timeseries/L=32,Lz=8/J=0.5"
# folderpath = "data/timeseries/L=32,Lz=8/J=1.0"

def Tseries(folderpath):
    n_sites = 32*32*8
    # n_sites = 16*16*8
    Ts, steps, Hs, delta2s = read_timeseries_folder(folderpath)

    avg_Hs = []     # per site
    # std_Hs = []
    avg_delta2s = []

    for i in range(len(Ts)):
        avg_Hs.append(Hs[i].mean() / (n_sites))
        # std_Hs.append(Hs[i].std(ddof=0))
        avg_delta2s.append(delta2s[i].mean())
    
    return (Ts, avg_Hs, avg_delta2s)

T_low,H_low,delta2_low = Tseries("data/timeseries/L=32,Lz=8/J=0.1")
T_mid,H_mid,delta2_mid = Tseries("data/timeseries/L=32,Lz=8/J=0.5")
T_hig,H_hig,delta2_hig = Tseries("data/timeseries/L=32,Lz=8/J=1.0")


plt.figure(1)
# ax[0].errorbar(Ts, avg_Hs, yerr=std_Hs, fmt="o")
plt.scatter(T_low, H_low, label="$J'/J=0.1$")
plt.scatter(T_mid, H_mid, label="$J'/J=0.5$")
plt.scatter(T_hig, H_hig, label="$J'/J=1.0$")
plt.title("Mean Energy")
plt.grid()
plt.legend()
plt.xlabel(r"$T \,/\, J$")
plt.ylabel(r"$\langle H \rangle \,/\, N$")
plt.savefig("figures/Tseries/energy.png")

plt.figure(2)
plt.scatter(T_low, delta2_low, label="$J'/J=0.1$")
plt.scatter(T_mid, delta2_mid, label="$J'/J=0.5$")
plt.scatter(T_hig, delta2_hig, label="$J'/J=1.0$")
plt.title("Layer Interconnectivity")
plt.ylabel(r"$\delta^2$")
plt.xlabel(r"$T \,/\, J$")
plt.grid()
plt.legend()
plt.savefig("figures/Tseries/delta2.png")