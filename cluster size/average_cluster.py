import numpy as np
import matplotlib.pyplot as plt

sqr100 = np.loadtxt("tri_100_avg.txt", skiprows=1)
order = np.argsort(sqr100[:,0])
sqr100[:,0] = np.array(sqr100[:,0])[order]
sqr100[:,1] = np.array(sqr100[:,1])[order]

sqr200 = np.loadtxt("tri_200_avg.txt", skiprows=1)
order = np.argsort(sqr200[:,0])
sqr200[:,0] = np.array(sqr200[:,0])[order]
sqr200[:,1] = np.array(sqr200[:,1])[order]

sqr300 = np.loadtxt("tri_300_avg.txt", skiprows=1)
order = np.argsort(sqr300[:,0])
sqr300[:,0] = np.array(sqr300[:,0])[order]
sqr300[:,1] = np.array(sqr300[:,1])[order]

plt.title("Average finite cluster size for triangular lattice")

plt.xlabel(r"Probability $p$")
plt.ylabel(r"Average cluster size $\overline{s}$")

plt.plot(sqr100[:,0], sqr100[:,1], label=r"$L=100$", marker=".", linestyle="-", markersize=3, color="tab:blue")
plt.plot(sqr200[:,0], sqr200[:,1], label=r"$L=200$", marker=".", linestyle="-", markersize=3, color="tab:orange")
plt.plot(sqr300[:,0], sqr300[:,1], label=r"$L=300$", marker=".", linestyle="-", markersize=5, color="tab:green")

plt.vlines(0.5, 1, 20, linestyle="--", color="dimgrey", label=r"$P_{\text{c}}^{\text{tab}} = 0.5$", zorder=-5)
plt.vlines(0.50143, 1, 20, linestyle="-", color="dimgrey", label=r"$P_{\text{c}} \approx 0.50143$", zorder = 10)

plt.legend()
plt.tight_layout()
plt.savefig("tri_avg.png", dpi=300, bbox_inches="tight")
plt.show()

plt.close()

exit()

tri = np.loadtxt("tri.txt")

plt.title("Average finite cluster size for triangular lattice")

plt.xlabel(r"Probability $p$")
plt.ylabel(r"Average cluster size")

plt.plot(tri[:,0], tri[:,1], label=r"$L=100$", marker="^", linestyle="-", markersize=5)
plt.plot(tri[:,0], tri[:,2], label=r"$L=200$", marker="^", linestyle="-", markersize=5)
plt.plot(tri[:,0], tri[:,3], label=r"$L=300$", marker="^", linestyle="-", markersize=5)

plt.vlines(0.5, 0, 26.5, linestyle="--", color="dimgrey", label=r"$P_{\text{c}} \approx 0.5$")

plt.legend()
plt.tight_layout()
plt.savefig("tri.png", dpi=300, bbox_inches="tight")
plt.show()