import numpy as np
import matplotlib.pyplot as plt

grid_file = np.genfromtxt("grid_positions.txt", delimiter=",")

pos_x = []
pos_y = []

for i in range(0,100):
    if (i % 2 == 0): 
        pos_x = np.append(pos_x,grid_file[i])
        #print(pos_x[i])
    else:
        pos_y = np.append(pos_y,grid_file[i])

plt.plot(pos_x, pos_y, 'r.')
plt.xlim((0,1))
plt.ylim((0,1))
plt.show()