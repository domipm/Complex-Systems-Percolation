import numpy as np
import matplotlib.pyplot as plt
import scipy.optimize as opt

# BELOW CRITICAL PROBABILITY

data = np.loadtxt("300_0.25.txt", skiprows=1)
vals = np.unique(data[:,1], return_counts=True)

plt.bar(vals[0], vals[1], width=0.9)
#plt.plot(vals[0], vals[1])
#plt.hist(x=data[:,1], bins=30)

plt.title(r"Average cluster size (Triangular lattice $p < p_c$)")
plt.xlabel(r"Number of nodes in cluster $n_s$")

plt.xticks([1,5,10,15,20,25,30,35,40])

plt.yscale("log")
#plt.xscale("log")

#plt.tight_layout()
plt.savefig("tri_below.png", dpi=300, bbox_inches="tight")
#plt.show()
plt.close()

# AT CRITICAL PROBABILITY

data = np.loadtxt("400_0.50.txt", skiprows=1)
vals = np.unique(data[:,1], return_counts=True)

fig, ax1 = plt.subplots()

# These are in unitless percentages of the figure size. (0,0 is bottom left)
left, bottom, width, height = [0.43, 0.4, 0.45, 0.45]
ax2 = fig.add_axes([left, bottom, width, height])

ax1.set_yscale("log")

ax2.set_xscale("log")
ax2.set_yscale("log")

ax2.text(x=100, y=850, s="log/log plot", fontsize=10)

ax1.set_title(r"Average cluster size (Triangular lattice $p \sim p_c$)")
ax1.set_xlabel(r"Number of nodes in cluster $n_s$")

ax1.hist(x=data[:,1], bins=30, rwidth=0.9)
ax2.bar(vals[0], vals[1], width=0.9)

ax2.set_ylim(0, 1.5*10e2)
#ax2.set_xlim(10e0, 10e3)

# Power law fit of values
def func(x, a, b):

    return a*x**-b


# Perform curve fit
popt, pcov = opt.curve_fit(func, vals[0], vals[1])

# Plot regression
print(popt)
ax2.plot(vals[0], func(vals[0], *popt), marker="", linestyle="-", linewidth=2, color="tab:orange")

#ax2.hist(x=data[:,1], bins=30, rwidth=0.9)

#plt.bar(vals[0], vals[1], width=0.9)
#plt.hist(x=data[:,1], bins=30, rwidth=0.9)
#plt.plot(vals[0], vals[1])

#plt.title(r"Average cluster size (Square lattice $P \sim P_c$)")
#plt.xlabel("Number of nodes in cluster")

#plt.yscale("log")
#plt.xscale("log")

#plt.tight_layout()
#plt.savefig("tri_at.png", dpi=300, bbox_inches="tight")
plt.show()
plt.close()

# ABOVE CRITICAL PROBABILITY

data = np.loadtxt("300_0.75.txt", skiprows=1)
vals = np.unique(data[:,1], return_counts=True)

f, (ax, ax2) = plt.subplots(1, 2, sharey=True, facecolor='w')

# plot the same data on both axes
ax.hist(x=data[:,1], bins=30, rwidth=0.9)
ax2.hist(x=data[:,1], bins=30, rwidth=0.9)

ax.set_xlim(0, 10000)
ax2.set_xlim(55000, 70000)

# hide the spines between ax and ax2
ax.spines['right'].set_visible(False)
ax2.spines['left'].set_visible(False)
ax2.yaxis.tick_right()

d = .015  # how big to make the diagonal lines in axes coordinates
# arguments to pass plot, just so we don't keep repeating them
kwargs = dict(transform=ax.transAxes, color='k', clip_on=False)
ax.plot((1-d, 1+d), (-d, +d), **kwargs)
#ax.plot((1-d, 1+d), (1-d, 1+d), **kwargs)

kwargs.update(transform=ax2.transAxes)  # switch to the bottom axes
#ax2.plot((-d, +d), (1-d, 1+d), **kwargs)
ax2.plot((-d, +d), (-d, +d), **kwargs)

#plt.bar(vals[0], vals[1])
#plt.hist(x=data[:,1], bins=30, rwidth=0.9)
#plt.plot(vals[0], vals[1])

#plt.title(r"Average cluster size (Square lattice $P > P_c$)")
#plt.xlabel("Number of nodes in cluster")

ax.set_yscale("log")
ax2.set_yscale("log")

#ax.set_xlabel("Number of nodes in cluster")
#ax.xaxis.set_label_coords(1.4, 0.12, transform=fig.transFigure)

ax.set_title(r"Average cluster size (Triangular lattice $p > p_c$)", loc="center").set_position((1.1,0))
ax.set_xlabel(r"Number of nodes in cluster $n_s$", loc="center").set_position((1.1,0))

ax.set_xticks([0,4000, 8000])
ax2.set_xticks([60000, 70000])

ax1.get_yaxis().set_visible(False)

plt.savefig("tri_above.png", dpi=300, bbox_inches="tight")
#plt.show()
plt.close()