import matplotlib.pyplot as plt
import numpy as np
from numpy import pi, sin, cos


u = np.linspace(0, 2*pi, 100)
v = np.linspace(0, pi, 100)
x = np.outer(cos(u), sin(v))
y = np.outer(sin(u), sin(v))
z = np.outer(np.ones(np.size(u)),cos(v))

fig = plt.figure()
ax = fig.add_subplot(projection="3d")
ax.plot_surface(x,y,z)
ax.set_aspect("equal")
plt.show()