import numpy as np
import matplotlib.pyplot as plt

sqr = np.loadtxt("sqr.txt")

plt.title("Average finite cluster size for square lattice")

plt.xlabel(r"Probability $p$")
plt.ylabel(r"Average cluster size")

plt.plot(sqr[:,0], sqr[:,1], label=r"$L=100$", marker="s", linestyle="-", markersize=5)
plt.plot(sqr[:,0], sqr[:,2], label=r"$L=200$", marker="s", linestyle="-", markersize=5)
plt.plot(sqr[:,0], sqr[:,3], label=r"$L=300$", marker="s", linestyle="-", markersize=5)

plt.vlines(0.59274, 0, 9.5, linestyle="--", color="dimgrey", label=r"$P_{\text{c}} \approx 0.59274$")

plt.legend()
plt.tight_layout()
plt.savefig("sqr.png", dpi=300, bbox_inches="tight")
plt.show()

plt.close()

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