import numpy as np
import matplotlib.pyplot as plt

from timeseries import read_different_Ts

# folderpath = "data/timeseries/L=8,Lz=4"
folderpath = "data/timeseries/L=16,Lz=8"
L = 16
Lz = 8
Ts, steps, Hs, H2s = read_different_Ts(folderpath)


cs = []
# the fraction of total samples that will be used in averaging.
# i.e. the last 20% of samples or whatever.
fraction_of_samples = 0.3
for i in range(len(Ts)):
    T = Ts[i]
    sample_from = int((1 - fraction_of_samples)*len(Hs[i]))

    # strategy 1: first average H and H^2, then find c. seems unreliable.
    # H = np.average(Hs[i][sample_from:])
    # H2 = np.average(H2s[i][sample_from:])
    # c = 1/(L**2 * Lz * T**2) * (H2 - H**2)
    # cs.append(c)

    # strategy 2: compute c for each entry of H and H^2. Then average c.
    H = Hs[i][sample_from:]
    H2 = H2s[i][sample_from:]
    c = np.average(1/(L**2 * Lz * T**2) * (H2 - H**2))
    cs.append(c)

plt.plot(Ts, cs, "bo")
plt.show()