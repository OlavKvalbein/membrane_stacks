import numpy as np
import matplotlib.pyplot as plt

from timeseries import read_different_Ts

folderpath = "data/timeseries/L=8,Lz=4"
Ts, steps, Hs, H2s = read_different_Ts(folderpath)
print(type(Hs[0]))
L = 8
Lz = 4

cs = []
equilibrium_Hs = []
for i in range(len(Ts)):
    T = Ts[i]

    average_from_sample_nr = 50
    H = np.average(Hs[i][average_from_sample_nr:])
    H2 = np.average(H2s[i][average_from_sample_nr:])

    c = 1/(L**2 * Lz * T**2) * (H2 - H**2)
    cs.append(c)
    equilibrium_Hs.append(H)

plt.plot(Ts, cs, "bo")
plt.show()

plt.plot(Ts, equilibrium_Hs, "bo")
plt.show()