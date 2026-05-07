import numpy as np
import matplotlib.pyplot as plt

from timeseries import read_timeseries_folder

folderpath = "data/timeseries/L=32,Lz=8/J=0.1"

Ts, steps, Hs, delta2s = read_timeseries_folder(folderpath)

avg_Hs = []
avg_delta2s = []

for i in range(len(Ts)):
    avg_Hs.append(Hs[i].mean())
    avg_delta2s.append(delta2s[i].mean())

fig, ax = plt.subplots(ncols=2)
ax[0].scatter(Ts, avg_Hs)
ax[1].scatter(Ts, avg_delta2s)
plt.show()