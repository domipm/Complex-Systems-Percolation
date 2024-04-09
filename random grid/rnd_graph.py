import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("rnd_out.txt", skiprows=1)

x = data[:,1]
y = data[:,2]

fig, ax = plt.subplots()
ax.set_aspect(1)
#circle = plt.Circle(( 0.6 , 0.6 ), 0.2, fill=False, color="Black" )

ax.set_xlim((0,10))
ax.set_ylim((0,10))

#ax.plot(x,y,marker=".",linestyle="")
sc = ax.scatter(x,y,c=data[:,4],cmap="Set1")

# Plot circles around each node indicating distance to neighbors
for i in range(len(x)):
    ax.add_artist( plt.Circle( (x[i], y[i]), 1, fill=False, color="Black", alpha=0.15 ) )

#plt.colorbar(sc)
plt.show()