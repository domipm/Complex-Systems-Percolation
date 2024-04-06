import matplotlib.pyplot as plt
import numpy as np

# Function to plot all points in lattice
def print_graph(nodes, name="", show=False):

    for i in range(len(nodes)): 
    
        #if (nodes[i].is_active == False): plt.polar(nodes[i].theta, nodes[i].r,marker='o',linestyle="", color="grey")
        if (nodes[i].is_active == True): plt.polar(nodes[i].theta, nodes[i].r, marker='.', linestyle="", color="red")
        if (nodes[i].is_tracking == True): plt.polar(nodes[i].theta, nodes[i].r,marker='.',linestyle="", color="blue")

    if (name != ""): plt.savefig(name, dpi=300)

    if (show == True): plt.show()

    plt.close()

# Node class
class Node(object):

    def __init__(self, r=0, theta=0, is_active=False, p=0, is_tracking=False):

        # Radial and angular coordinates
        self.r = r
        self.theta = theta
        # Whether the node is active star-formation node
        self.is_active = is_active
        # Tracking point
        self.is_tracking = is_tracking

# Array of nodes
nodes = []

# Probability of initially active
P0 = 0.01
# Number of radial rings
R0 = 49

# Generate initial state
for r in np.arange(2,R0,1):

    for theta in np.linspace(0,2*np.pi,7*r):

        node = Node(r,theta)
        if (np.random.uniform(0,1) < P0): node.is_active = True
        nodes = np.append(nodes, node)

nodes[15].is_tracking = True
nodes[36].is_tracking = True

# Print initial step
print_graph(nodes, "initial", show=False)

# Number of iterations
N = 15
# Probability of star-formation propagation
P = 0.18
# Velocity
V = 0.05
# Distance to nearest neighbor check
D = 2

# Perform percolation and rotation
for n in range(N):

    # Percolation
    for i in range(len(nodes)): 
        for j in range(len(nodes)):
            if (j != i):
                distance = nodes[i].r**2 + nodes[j].r**2 - 2*nodes[i].r*nodes[j].r*np.cos(nodes[i].theta - nodes[j].theta)
                if (distance <= D and np.random.uniform(0,1) < P): nodes[j].is_active = True 
    # Rotation
    for i in range(len(nodes)): 
        nodes[i].theta = nodes[i].theta + V * nodes[i].r

# Print final step
print_graph(nodes, "final")

exit()

# Perform one percolation step
for i in range(len(nodes)): 
    
    for j in range(len(nodes)):

        if (j != i):

            distance = nodes[i].r**2 + nodes[j].r**2 - 2*nodes[i].r*nodes[j].r*np.cos(nodes[i].theta - nodes[j].theta)
            if (distance <= 2 and np.random.uniform(0,1) < 0.05): nodes[j].is_active = True 

print_graph(nodes, "test2")

# Rotate nodes

for i in range(len(nodes)):

    nodes[i].theta = nodes[i].theta + 0.25 * nodes[i].r

print_graph(nodes, "test3")

exit()

#plt.show()
plt.savefig("test2.png")
plt.close()

exit()

for i in range(len(nodes)): 
    
    if (nodes[i].is_active == False): plt.polar(nodes[i].theta, nodes[i].r,marker='o',linestyle="", color="grey")
    else: plt.polar(nodes[i].theta, nodes[i].r, marker='o', linestyle="", color="red")

plt.show()
plt.savefig("test1.png")
plt.close()

exit()

# Perform next step of simulation: find nearest neighbours, activate or not, rotate (later to implement)

for i in range(len(nodes)):

    for j in range(len(nodes)):

            if (i != j):
                distance = nodes[i].r**2 + nodes[j].r**2 - 2*nodes[i].r*nodes[j].r*np.cos(nodes[i].theta - nodes[j].theta)
                print(distance)
                if (distance < 1): nodes[j].is_active = True

for i in range(len(nodes)): 
    
    if (nodes[i].is_active == False): plt.polar(nodes[i].theta, nodes[i].r,marker='o',linestyle="", color="grey")
    else: plt.polar(nodes[i].theta, nodes[i].r, marker='o', linestyle="", color="red")

plt.savefig("test2.png")

plt.close()

exit()

for r in np.arange(2,12,1): # Radial coordinate

    theta = np.linspace(0,2*np.pi,6*r) # 
    plt.polar(theta,np.full(len(theta),r),marker='o',linestyle="")

exit()

# plotting the polar coordinates on the system
#plt.polar(theta,r,marker='o',linestyle="")

# Displaying the plot
plt.plot()
plt.show()
