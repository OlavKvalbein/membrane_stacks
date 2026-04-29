import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os
import glob

def read_sampling(path):
    df = pd.read_csv(path, comment="#", index_col=False)
    return df

def read_different_Ts(folderpath):
    # only certain csv files.
    paths = glob.glob(folderpath + "/T=*****.csv")
    # extract the Ts from the filenames
    Ts = [float(os.path.basename(path)[2:-4]) for path in paths]

    steps = []
    Hs = []
    for path in paths:
        df = read_sampling(path)
        steps.append(df["step"])
        Hs.append(df["H"])
    return (Ts, Hs, steps)

Ts, Hs, steps = read_different_Ts("data/energy_timeseries")

for i in range(len(Ts)):
    plt.plot(steps[i], Hs[i], label=f"T = {Ts[i]}")

plt.legend()
plt.show()