import numpy as np
import matplotlib.pyplot as plt
import random

D = 1.1

data = np.loadtxt("sq_grid_positions.txt", skiprows=1)

x = data[:,0] # X position
y = data[:,1] # Y position
s = data[:,2] # Status (active/inactive)
c = data[:,3] # cluster_index

fig, ax = plt.subplots()
ax.set_aspect(1)

ax.set_xlim((-1,10))
ax.set_ylim((-1,10))

# Plot all points
ax.scatter(x,y,c=c,cmap="rainbow")
# Plot inactive nodes as black points
for i in range(len(x)):
    if (s[i] == 0):
        ax.scatter(x[i],y[i],color="black")
    if (s[i] == 1):
        #ax.scatter(x[i],y[i],color="red")
        ax.add_artist( plt.Circle( (x[i], y[i]), D, fill=False, color="black", alpha=0.15 ) )

plt.show()