#include<iostream>
#include<fstream>
#include<cmath>
#include<string>

#include<stdio.h>

#include"gsl_rng.h"

// Maximum size of arrays
#define N_MAX 100

// Length of square inside which nodes are generated
int L = 10;
// Nodes to generate
int N = 100;
// Random seed
int SEED = 6857349;
// Initial probability
float P = 0.65;
// Distance threshold
float D = 2;
// Percolation probability
float p = 0.25;

// Random variable pointer
gsl_rng *tau;
// File pointer
FILE *file;

class Node {

    public:

        // Position of node
        float x = .0;
        float y = .0;
        // Active-or-not status
        bool is_active = false;
        // Node index to identify
        int index = 0;
        // Array of indices of all neighbors (max. size equals to all nodes)
        int neighbors[N_MAX] = {-1};
        // Index of cluster node is in (default -1 cluster is none)
        int cluster_index = -1;

        // Function to output useful parameters
        void print_params() {

            printf("index=%02i\tx=%.5f\ty=%.5f\tis_active=%i\tcluster_index=%i\n",index,x,y,is_active,cluster_index);
            return;

        }

        // Function to print all neighbor indices of the node
        void print_neighbors() {
            // Run over all nodes
            for (int i = 0; i < N; i++)
                if (neighbors[i] >= 0) // Exclude auxiliary index values
                    printf("%i\n",neighbors[i]);
            return;
        }

        // Function to set all neighbors to the same cluster index
        void set_cluster_neighbors(Node nodes[N_MAX]) {
            for (int i = 0; i < N; i++) if (neighbors[i] >= 0) nodes[neighbors[i]].cluster_index = cluster_index;
            return;
        }

        // Function to find neighbors of node
        void find_neighbors(Node nodes[N_MAX]) {
            // Go over all nodes
            for (int i = 0; i < N; i++) {
                // Calculate distance
                float dist = sqrt( (nodes[i].x - x)*(nodes[i].x -x) + (nodes[i].y - y)*(nodes[i].y - y) );
                if (dist <= D && dist != 0 && is_active == true) // If distance is not zero (itself) and less than threshold and node active
                    neighbors[i] = nodes[i].index; // Add neighbors to neighbor array (except zero)
                else neighbors[i] = -1;
            }
            return;
        }

};

class Cluster {

    public:

        // Array of nodes inside cluster
        Node node_inside[N_MAX];

        // Current cluster's index
        int index = -1;

        // Function to check whether given node is inside the cluster
        bool inside_cluster(Node node) {

            for (int i = 0; i < N_MAX; i++)
                if (node.index == node_inside[i].index) return true;
            
            return false;

        }

        // Function to add node inside cluster
        void add_node(Node &node) {

            for (int i = 0; i < N_MAX; i++) 
                if (node_inside[i].cluster_index == -1) {

                    node_inside[i] = node; // Add to first non-empty position
                    node.cluster_index = index; // Set node's cluster index to the current cluster's index
                
                }

            return;

        }

        // Function to print node indices of all (no null) nodes inside cluster
        void print_nodes_in(Node node[N_MAX]) {

            for (int i = 0; i < N_MAX; i++)
                if (inside_cluster(node[i]))
                    printf("%02i\n", node_inside[i].index);

            return;

        }

};

// Function that returns length of array (-1 values ignored)
int len(int a[N_MAX]) {

    int l = 0;
    for (int i = 0; i < N; i++) if (a[i] >= 0) l += 1;
    return l;

}

// Function to check whether a value is in an array
bool in_array(int a[N_MAX], int b) {

    for (int i = 0; i < N_MAX; i++)
        if (a[i] == b) return true;

    return false;
}

// Function that prints all parameters of all the values in nodes array
void print_all(Node nodes[N_MAX]) {

    for (int n = 0; n < N; n++) nodes[n].print_params();
    return;

}

// Function that prints all parameters of all values in nodes array in file
void write_all(Node nodes[N_MAX]) {

    file = fopen("rnd_out.txt", "w");
    fprintf(file, "index\tx\ty\tis_active\tcluster_index\n");
    for (int n = 0; n < N; n++) fprintf(file, "%02i\t%.5f\t%.5f\t%i\t%i\n",nodes[n].index,nodes[n].x,nodes[n].y,nodes[n].is_active,nodes[n].cluster_index);

    return;

}

// Function that categorizes a given node into a cluster given its active nearest neighbors
void find_cluster(Node node, Node nodes[N_MAX], int iter) {

    // Find the neighbors of the node (only the active ones)
    node.find_neighbors(nodes);
    node.print_neighbors();

    // Set node's cluster index to iteration
    node.cluster_index = iter;

    // Set node's neighbors to same index
    for (int i = 0; i < N_MAX; i++) { 
        // Check if any neighbor already is in cluster, if so set its cluster index
        if (nodes[node.neighbors[i]].cluster_index != -1) node.cluster_index = nodes[node.neighbors[i]].cluster_index;
        else nodes[node.neighbors[i]].cluster_index = iter;
    }

    return;

}

int main(void) {

    // Initialize random number generator
    extern gsl_rng *tau;
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau, SEED);

    // Array of all nodes
    Node nodes[N];
    // Array of all clusters (at most one per node i.e. all disconnected)
    Cluster clusters[N];

    // Initialize all nodes
    for (int n = 0; n < N; n++) {
        // Set the positions (random between 0 and L)
        nodes[n].x = gsl_rng_uniform(tau)*L;
        nodes[n].y = gsl_rng_uniform(tau)*L;
        // Set index of all nodes
        nodes[n].index = n;
        // Set index of all clusters
        clusters[n].index = n;
        // Activate randomly some nodes with probability p0
        if (gsl_rng_uniform(tau) < P) 
            nodes[n].is_active = true;
    }

    // Find the neighbors of all nodes
    for (int n = 0; n < N; n++)
        nodes[n].find_neighbors(nodes);

    // Set the first active node to the first cluster
    int nfirst = 0;
    for (nfirst = 0; nfirst < N; nfirst++)
        if (nodes[nfirst].is_active == true) {
            clusters[0].add_node(nodes[nfirst]);
            break;
        }

    //print_all(nodes);

    //find_cluster(nodes[0], nodes, 0);

    //for (int n = 0; n < N; n++) 
    //    find_cluster(nodes[n], nodes, n);

    write_all(nodes);

    /*
    
    //print_all(nodes);

    // Go over all nodes
    int c = 1; // Initial cluster index
    for (int n = 0; n < N-1; n++) {

        // Check for active node, set its cluster index, find neighbors and set their cluster number
        if (nodes[n].is_active == true && nodes[n].is_checked == false) {
            nodes[n].cluster_index = c; // Set cluster index
            nodes[n].find_neighbors(nodes); // Look for his neighbors
            if (len(nodes[n].neighbors) > 1) // Array length greater than one (previous neighbor) and active node
                for (int i = 0; i < N; i++)
                    if (nodes[nodes[n].neighbors[i]].is_active == true) {
                        nodes[nodes[n].neighbors[i]].cluster_index = nodes[n].cluster_index;
                        nodes[nodes[n].neighbors[i]].is_checked = true;
                    } 
        }

        // Check for next value, if it is zero (disconnected) increase cluster index by one
        if (nodes[n+1].is_active == false) c += 1;

    }

    print_all(nodes);

    /*
    
    // Find the first non-zero entry and set cluster_index = 1
    int n = 0;
    while (n < N) {

        if (nodes[n].is_active == true) {
            nodes[n].cluster_index = 1;
            break;
        } else n += 1;

    }

    // Compute the neighbors of the first non-zero entry
    nodes[n].find_neighbors(nodes);
    nodes[n].print_neighbors();
    
    // Set the cluster_index of all neighbors to same value if they are active
    for (int i = 0; i < N; i++) {

        if (nodes[nodes[n].neighbors[i]].is_active == true) nodes[nodes[n].neighbors[i]].cluster_index = nodes[n].cluster_index;

    }

    */

    return 0;

}