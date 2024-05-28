import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("forest_results.txt", skiprows=1)

plt.plot(data[:,0], data[:,1], label="R = 1.00")
plt.plot(data[:,0], data[:,2], label="R = 0.75")
plt.plot(data[:,0], data[:,3], label="R = 0.50")
plt.plot(data[:,0], data[:,4], label="R = 0.33")
plt.plot(data[:,0], data[:,5], label="R = 0.25")
plt.plot(data[:,0], data[:,6], label="R = 0.15")
plt.plot(data[:,0], data[:,7], label="R = 0.10")
plt.plot(data[:,0], data[:,8], label="R = 0.05")
plt.plot(data[:,0], data[:,9], label="R = 0.00")

#plt.vlines([0.2401, 0.2442, 0.2600, 0.2905, 0.3104, 0.3696, 0.4097, 0.5780], 1, 25, color="dimgrey", linestyle="--")

plt.title("Average cluster size random forest")

plt.xlabel(r"Probability $p$")
plt.ylabel(r"Average cluster size $\overline{s}$")

plt.legend()
plt.savefig("forest.png", dpi=300, bbox_inches="tight")
plt.show()