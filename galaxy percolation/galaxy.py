import matplotlib.pyplot as plt
import numpy as np
import time
import pandas as pd

# Function to plot all points in lattice
def print_graph(nodes, name="", show=False, show_inactive = False):

    for i in range(len(nodes)): 
        # Plot inactive nodes
        if (nodes[i].is_active == False and show_inactive == True): plt.polar(nodes[i].theta, nodes[i].r,marker='.',linestyle="", color="grey")
        # Plot active nodes
        if (nodes[i].is_active == True): plt.polar(nodes[i].theta, nodes[i].r, marker='.', linestyle="", color="red")
        # Plot tracked nodes (always on top)
        if (nodes[i].is_tracking == True): plt.polar(nodes[i].theta, nodes[i].r, marker='.', linestyle="", color="blue")
        # Plot neighboring nodes (always on top)
        if (nodes[i].is_neighbor == True): plt.polar(nodes[i].theta, nodes[i].r, marker='.', linestyle="", color="green")
    if (name != ""): plt.savefig(name, dpi=300) # Output graph to file if name provided
    if (show == True): plt.show() # Show graph on screen

    plt.close() # Close previous plot before plotting next

# Node class
class Node(object):

    def __init__(self, r = 0, theta = 0, is_active = False, is_tracking = False, is_neighbor = False):

        # Radial and angular coordinates
        self.r = r
        self.theta = theta
        # Whether the node is active star-formation node
        self.is_active = is_active
        # Tracking point
        self.is_tracking = is_tracking
        # Tracking neighboring points
        self.is_neighbor = is_neighbor

# Computation time measure
start = time.time()

# Array of all nodes
nodes = []
# Array of all active nodes
active_nodes = []

# Probability of initially active
P0 = 0.0075
# Number of radial rings (~ total radius of galaxy)
R0 = 50

# Generate all nodes, initially non-active
# Radius between 2 and total radius R0
for r in np.arange(2,R0):
    # Angular coordinate between 0 and 2pi, for each radius generate 7*r nodes
    for theta in np.linspace(0,2*np.pi,7*r):
        # Create the node
        node = Node(r,theta) 
        # Randomly activate some nodes
        if (np.random.uniform(0,1) < P0): 
            node.is_active = True
            active_nodes = np.append(active_nodes, node)
        # Add node to nodes array
        nodes = np.append(nodes, node)

# Print initial step
print_graph(nodes, "testi.png")

# Store r,theta positions in separate arrays
r_array = []
theta_array = []
for i in range(len(nodes)):
    r_array = np.append(r_array, nodes[i].r)
    theta_array = np.append(theta_array, nodes[i].theta)
# Create dataframe to store positions of all nodes
df = pd.DataFrame({"r" + str(0): r_array, "theta" + str(0): theta_array})

# Number of iterations
N = 10
# Probability of star-formation propagation
P = 0.25
# Velocity (radial-dependent rotation)
V = 55
# Distance to nearest neighbor check
D = 1.25

for n in range(1,N+1):
    print("Iteration " + str(n))
    # Percolation
    timer = time.time()
    for i in range(len(active_nodes)):
        for j in range(len(nodes)):
            distance = np.sqrt( active_nodes[i].r**2 + nodes[j].r**2 - 2*active_nodes[i].r*nodes[j].r*np.cos(active_nodes[i].theta - nodes[j].theta))
            if (distance <= D and distance != 0 and np.random.uniform(0,1) <= P): 
                nodes[j].is_active = True 
                active_nodes = np.append(active_nodes, nodes[j])
    print("End of percolation " + str(time.time() - timer))
    # Rotation
    for i in range(len(nodes)): nodes[i].theta += V / np.sqrt(nodes[i].r**3)

    # Append iteration to dataframe
    if (n % 5 == 1):
        # Obtain r,theta positions of all nodes
        r_array = []
        theta_array = []
        for i in range(len(nodes)):
            r_array = np.append(r_array, nodes[i].r)
            theta_array = np.append(theta_array, nodes[i].theta)
        # New temporary dataframe
        df_temp = pd.DataFrame({"r" + str(n): r_array, "theta" + str(n): theta_array})
        # Concatenate dataframes
        df = df.join(df_temp, how="right")

# Show final graph
print_graph(nodes, "testf", show=False)

# Write output csv file
df.to_csv("galaxy_output.csv")

# Display total computational time
end = time.time()
print("Computational time: " + str(end - start))