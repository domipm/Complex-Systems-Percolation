import numpy as np
import matplotlib.pyplot as plt
import random

D = 1.25
show_circles = True
show_index = False
show_graph = False
show_inactive = True

# PLOT INITIAL STATE

data_init = np.loadtxt("rnd_out_init.txt", skiprows=1)

# Initialize plot
fig = plt.figure()
ax = fig.add_subplot(projection='3d',computed_zorder=False)
# Set limits and other parameters
ax.set_xlim((0,8))
ax.set_ylim((0,8))
ax.set_zlim((0,8))
#ax.set_aspect('auto')
for axis in [ax.xaxis, ax.yaxis, ax.zaxis]: axis.set_pane_color((1, 1, 1))

# Size of marker ~ circles
points_whole_ax = 5 * 0.8 * 72    # 1 point = dpi / 72 pixels
radius = 1.5
points_radius = 2 * radius / 1.0 * points_whole_ax

for i in range(len(data_init[:,0])):
    if (data_init[i,4] == 0): # Plot inactive nodes as black points
        ax.scatter(data_init[i,1],data_init[i,2],data_init[i,3],color="black",zorder=2)
    if (data_init[i,4]== 1 and show_circles == True): # Plot circles around active points
        ax.scatter(data_init[i,1],data_init[i,2],data_init[i,3],marker='o',linestyle="",color="black",depthshade=False, zorder = -2, alpha=0.15, s=points_radius)
# Plot all points
ax.scatter(data_init[:,1],data_init[:,2],data_init[:,3],marker='.',linestyle="", color="orangered", depthshade=False, zorder=0, s=80)
# Plot labels
if (show_index == True):
    for i in range(len(data_init[:,0])):
        ax.text(x = data_init[i,1] + 0.1, y = data_init[i,2] + 0.1, z = data_init[i,3] + 0.1, s = int(data_init[i,0]), size=5)

plt.savefig("testing_init.png", dpi=300, bbox_inches='tight')
#plt.show()
plt.close()

# PLOT FINAL STATE

data = np.loadtxt("rnd_out.txt", skiprows=1)

# Initialize plot
fig = plt.figure()
ax = fig.add_subplot(projection='3d',computed_zorder=False)
# Set limits and other parameters
ax.set_xlim((0,7))
ax.set_ylim((0,7))
ax.set_zlim((0,7))
#ax.set_aspect('auto')
for axis in [ax.xaxis, ax.yaxis, ax.zaxis]: axis.set_pane_color((1, 1, 1))

# Size of marker ~ circles
points_whole_ax = 5 * 0.8 * 72    # 1 point = dpi / 72 pixels
radius = 3
points_radius = 2 * radius / 1.0 * points_whole_ax
'''
for i in range(len(data_init[:,0])):
    if (data_init[i,4] == 0): # Plot inactive nodes as black points
        if (show_inactive == True): ax.scatter(data_init[i,1],data_init[i,2],data_init[i,3],color="black",zorder=2)
        else: ax.scatter(data_init[i,1],data_init[i,2],data_init[i,3],color="black", alpha=0,zorder=2)
    if (data_init[i,4]== 1 and show_circles == True): # Plot circles around active points
        ax.scatter(data_init[i,1],data_init[i,2],data_init[i,3],marker='o',linestyle="",color="black",depthshade=False, zorder = -2, alpha=0.05, s=points_radius)
# Plot all points
#ax.scatter(x=data[:,1],y=data[:,2],z=data[:,3],c=data[:,4],marker='.',linestyle="", cmap="rainbow", depthshade=False, zorder=0, s=80)
'''
for i in range(len(data[:,0])): ax.scatter(data[i,1],data[i,2],data[i,3],marker='o',linestyle="",color="black",depthshade=False, zorder = -2, alpha=0.025, s=points_radius)
ax.scatter(data[:,1], data[:,2], data[:,3], c=data[:,5], cmap="rainbow", marker='o', alpha=1)

# Plot labels
if (show_index == True):
    for i in range(len(data[:,0])):
        ax.text(x = data[i,1] + 0.1, y = data[i,2] + 0.1, z = data[i,3] + 0.1, s = int(data[i,0]), size=5)

plt.savefig("testing.png", dpi=300, bbox_inches='tight')
#plt.show()