import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("data/energy.csv", comment="#", index_col=False)

plt.plot(df["H"])
plt.show()