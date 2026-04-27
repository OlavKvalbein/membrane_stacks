import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def read_sampling(path):
    df = pd.read_csv(path, comment="#", index_col=False)
    return df

df = read_sampling("data/energy_timeseries/T=2.000.csv")
plt.plot(df["step"], df["H"])
plt.show()