import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import UnivariateSpline


from timeseries import read_timeseries_folder

# folderpath = "data/timeseries/L=32,Lz=8/J=0.1"
# folderpath = "data/timeseries/L=32,Lz=8/J=0.5"
# folderpath = "data/timeseries/L=32,Lz=8/J=1.0"

def Tseries(folderpath):
	n_sites = 32*32*8
	# how many temperatures to ignore
	cutoff = 7
	# n_sites = 16*16*8
	Ts, steps, Hs, delta2s = read_timeseries_folder(folderpath)

	avg_Hs = []     # per site
	# std_Hs = []
	avg_delta2s = []

	for i in range(cutoff, len(Ts)):
		avg_Hs.append(Hs[i].mean() / (n_sites))
		avg_delta2s.append(delta2s[i].mean())
	
	return (np.array(Ts[cutoff:]), np.array(avg_Hs), np.array(avg_delta2s))

T_low,H_low,delta2_low = Tseries("data/timeseries/L=32,Lz=8/J=0.1")
T_mid,H_mid,delta2_mid = Tseries("data/timeseries/L=32,Lz=8/J=0.5")
T_high,H_high,delta2_high = Tseries("data/timeseries/L=32,Lz=8/J=1.0")

plt.rcParams.update({
	# font stuff
	"font.family": "serif",
	"font.serif": "Linux Libertine",  
	"font.size":       14,
	"axes.titlesize": 20,
	"axes.labelsize": 16,
	"legend.fontsize": 14,
	"xtick.labelsize": 10,
	"ytick.labelsize": 10,
	"mathtext.fontset": "stix",
	# other
	"axes.linewidth":  0.8,
	"legend.frameon":  True,
	"legend.framealpha": 0.9,
	"axes.grid": True,
	"grid.linewidth":  0.5,
	"grid.alpha":      0.25,
	"figure.facecolor":"#fdfbf8",
})
plt.rcParams["axes.prop_cycle"] = plt.cycler(color=[
	"#ff8b5c", "#7fd1ae",'#2b2d42'
])

def small_arrows(ax, xs, ys):
	for x, y in zip(xs, ys):
		ax.annotate("",(x, y),(x,y+0.2),
			arrowprops=dict(arrowstyle="->", color="black", lw=1.5))

light_gray = "#d3d3d3"		

# Energy    ===========================================
fig1, ax1 = plt.subplots()
ax1.scatter(T_low, H_low, label=r"$J_z\,/\,J\,=\,0.1$")
ax1.scatter(T_mid, H_mid, label=r"$J_z\,/\,J\,=\,0.5$")
ax1.scatter(T_high, H_high, label=r"$J_z\,/\,J\,=\,1.0$")
# small_arrows(ax1, [infl[0], infl[1], infl[2]])

# Splines to estimate inflection points
# spline_low = UnivariateSpline(T_low, H_low, s=0.005)
# spline_mid = UnivariateSpline(T_mid, H_mid, s=0.005)
# spline_high = UnivariateSpline(T_high, H_high, s=0.005)
# x = np.linspace(1.5, 5.0, 1000)
# ax1.plot(x, spline_low(x))
# ax1.plot(x, spline_mid(x))
# ax1.plot(x, spline_high(x))

# approximate inflection points
infl_T = [2.546, 3.45, 4.228]
# infl_H = [-1.246, -1.22, -1.399]

for T in infl_T:
	ax1.axvline(x=T, color=light_gray, linestyle='--')

ax1.set_title("Energy")
ax1.legend()
ax1.set_xlabel(r"$T \,/\, J$")
ax1.set_ylabel(r"$\langle H \rangle \,/\, N$")
fig1.savefig("figures/Tseries/energy.png")


# =====================================================


# delta2    ===========================================
fig2, ax2 = plt.subplots()
ax2.scatter(T_low, delta2_low, label=r"$J_z\,/\,J\,=\,0.1$")
ax2.scatter(T_mid, delta2_mid, label=r"$J_z\,/\,J\,=\,0.5$")
ax2.scatter(T_high, delta2_high, label=r"$J_z\,/\,J\,=\,1.0$")

for T in infl_T:
	ax2.axvline(x=T, color=light_gray, linestyle='--')

ax2.set_title("Layer Interconnectivity")
ax2.legend()
ax2.set_xlabel(r"$T \,/\, J$")
ax2.set_ylabel(r"$\delta^2$")
fig2.savefig("figures/Tseries/delta2.png")
# =====================================================
