import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os
import glob

def read_timeseries(path):
    df = pd.read_csv(path, comment="#", index_col=False)
    return df

def read_different_Ts(folderpath):
    # only certain csv files.
    paths = glob.glob(folderpath + "/T=*****.csv")
    # extract the Ts from the filenames
    Ts = [float(os.path.basename(path)[2:-4]) for path in paths]

    steps = []
    Hs = []
    H2s = []
    for path in paths:
        df = read_timeseries(path)
        steps.append(df["step"].to_numpy())
        Hs.append(df["H"].to_numpy())
        H2s.append(df["H^2"].to_numpy())

    return (Ts, steps, Hs, H2s)

if __name__ == "__main__":
    # Plots the energy at the different temperatures
    Ts, steps, Hs, _ = read_different_Ts("data/timeseries/L=8,Lz=4")

    for i in range(len(Ts)):
        plt.plot(steps[i], Hs[i], label=f"T = {Ts[i]}")

    plt.legend()
    plt.show()