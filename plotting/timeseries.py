import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os
import glob

def read_timeseries(path):
    df = pd.read_csv(path, comment="#", index_col=False)
    return df

# reads all files of name "T=*****.csv" in a folder
# steps, Hs, and delta2s are lists of numpy arrays
def read_timeseries_folder(folderpath):
    # only certain csv files.
    paths = glob.glob(folderpath + "/T=*****.csv")
    # extract the Ts from the filenames
    Ts = [float(os.path.basename(path)[2:-4]) for path in paths]

    steps = []
    Hs = []
    delta2s = []
    for path in paths:
        df = read_timeseries(path)
        steps.append(df["step"].to_numpy())
        Hs.append(df["H"].to_numpy())
        delta2s.append(df["delta^2"].to_numpy())

    return (Ts, steps, Hs, delta2s)

# plots the timeseries in one csv file
def plot_timeseries(filepath):
    df = read_timeseries(filepath)
    H = df["H"].to_numpy()
    delta2 = df["delta^2"].to_numpy()
    step = df["step"].to_numpy()

    fig, ax = plt.subplots(ncols=2, nrows=1)
    ax[0].plot(step, H)
    ax[1].plot(step, delta2)

    ax[0].set_title("Hamiltonian $H$")
    ax[1].set_title(r"$\delta^2$")
    plt.savefig(filepath + ".png")
    
# plots timeseries in a folder.
def plot_timeseries_folder(folderpath):
    Ts, steps, Hs, delta2s = read_timeseries_folder(folderpath)

    fig, ax = plt.subplots(ncols=2, nrows=1)

    for i in range(len(Ts)):
        ax[0].plot(steps[i], Hs[i], label=f"T = {Ts[i]}")

    for i in range(len(Ts)):
        ax[1].plot(steps[i], delta2s[i], label=f"T = {Ts[i]}")

    ax[0].set_title("Hamiltonian $H$")
    ax[1].set_title(r"$\delta^2$")
    plt.legend()
    plt.savefig(folderpath + "/figure.png")

if __name__ == "__main__":
    # folderpath = "data/timeseries/L=8,Lz=4"
    # folderpath = "data/timeseries/L=16,Lz=8"

    testpath = "data/timeseries/test.csv"
    plot_timeseries(testpath)

    # folderpath = "data/timeseries/test"
    # plot_timeseries_folder(folderpath)