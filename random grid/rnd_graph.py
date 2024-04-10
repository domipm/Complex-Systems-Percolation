import numpy as np
import matplotlib.pyplot as plt
import random

D = 1.5
show_circles = True
show_index = True
show_graph = False

data = np.loadtxt("rnd_out.txt", skiprows=1)

x = data[:,1] # X position
y = data[:,2] # Y position
s = data[:,3] # Status (active/inactive)
c = data[:,4] # cluster_index

fig, ax = plt.subplots()
ax.set_aspect(1)

ax.set_xlim((0,10))
ax.set_ylim((0,10))

# Plot all points
ax.scatter(x,y,c=c,cmap="rainbow")
# Plot inactive nodes as black points
for i in range(len(x)):
    if (s[i] == 0):
        ax.scatter(x[i],y[i],color="black")
    if (s[i] == 1 and show_circles == True):
        #ax.scatter(x[i],y[i],color="red")
        ax.add_artist( plt.Circle( (x[i], y[i]), D, fill=False, color="black", alpha=0.15 ) )

if show_index == True:
    n = []
    for i in range(len(x)):
        n = np.append(n, str(i))
    for i,n in enumerate(n):
        ax.annotate(n,(x[i],y[i]),size=6, xytext = (x[i]+0.1,y[i]+0.1))

plt.savefig("testing.png", dpi=300)
if (show_graph): plt.show()