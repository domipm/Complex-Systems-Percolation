#include<iostream>
#include<fstream>
#include<cmath>
#include<chrono>
#include<stdio.h>
#include <time.h>

#include"gsl_rng.h"

// Maximum size of arrays
#define N_MAX 1000

// Length of square inside which nodes are generated
int L = 8;
// Nodes to generate
int N = L*L*L;
// Random seed
int SEED = time(NULL);
// Initial probability
float P = 0.25;
// Distance threshold
float D = 1;

// Random variable pointer
gsl_rng *tau;
// File pointer
FILE *file;

class Node {

    public:

        /* PARAMETERS */

        // Position of node
        float x, y, z;
        // Active-or-not status
        bool is_active;
        // Node index and cluster index it belongs to
        int index, cluster_index;
        // Array of indices of all neighbors (all -1 except index at position where neighbor is)
        int neighbors[N_MAX] = {-1};
        // Number of active neighbors of node
        int n_neighbors;

        // Default constructor
        Node() {
            x = 0;
            y = 0;
            z = 0;
            is_active = false;
            index = 0;
            cluster_index = -1;
            n_neighbors = 0;
            for (int i = 0; i < N_MAX; i++) neighbors[i] = -1;
        }

        /* PRINTING DATA */

        // Function to output useful parameters of the node
        void print_params() {
            printf("index=%02i\tx=%.5f\ty=%.5f\tz=%.5f\tis_active=%02i\tcluster_index=%02i\n",index,x,y,z,is_active,cluster_index);
            return;
        }

        // Function to print all neighboring indices (and some useful parameters) of the node
        void print_neighbors(Node nodes[N_MAX]) {
            // Run over all nodes
            for (int i = 0; i < N; i++)
                if (neighbors[i] != -1) // Exclude auxiliary index values (-1)
                    printf("neighbor_index=%02i\tcluster_index=%02i\n",neighbors[i], nodes[neighbors[i]].cluster_index);
            return;
        }

        /* CALCULATIONS */

        // Function to find (active) neighbors of node and count them
        void find_neighbors(Node nodes[N_MAX], int D) {
            // If current node is inactive, return
            if (is_active == false) return;
            // Run over all nodes in the system
            for (int i = 0; i < N; i++) {
                // Calculate distance between current node and all others
                float dist = sqrt( (nodes[i].x - x)*(nodes[i].x -x) + (nodes[i].y - y)*(nodes[i].y - y) + (nodes[i].z - z)*(nodes[i].z - z) );
                // If the distance is small (non-zero) and the other node is active
                if (dist <= D && dist != 0 && nodes[i].is_active == true)
                    neighbors[i] = nodes[i].index; // Add neighbors to neighbor array
                // If the distance if far away or other node inactive, ignore
            }
            // Count the number of active neighbors found
            for (int i = 0; i < N; i++)
                if (neighbors[i] != -1 && nodes[neighbors[i]].is_active == true) n_neighbors++;
            return;
        }

        // Function to find whether any (at least one) neighbor of current node is labeled
        bool lab_neighbors(Node nodes[N_MAX]) {
            for (int i = 0; i < N; i++)
                if (neighbors[i] != -1)
                    if ( nodes[neighbors[i]].cluster_index != -1 ) return true;
            return false;
        }

        // Function to set all neighbors to the same cluster index (the minimum value between current node and neighbors)
        void set_cluster_neighbors(Node nodes[N_MAX]) {
            if (n_neighbors != 0) {

                for (int i = 0; i < N; i++) {

                    if (neighbors[i] >= 0) {
                        // Obtain minimum value of cluster index between current node and neighbors
                        int cl_min = std::min(cluster_index, nodes[neighbors[i]].cluster_index);
                        // Set neighboring node cluster indices to minimum
                        nodes[neighbors[i]].cluster_index = cl_min;
                        // Set current node cluster index to this same minimum
                        cluster_index = cl_min;
                    }

                }

            }
            return;
        }

        // Function to obtain the minimum value of cluster_index of neighboring nodes
        int min_neighbor_label(Node nodes[N_MAX]) {
            // If the node has no neighbors, return his own cluster index
            if (n_neighbors == 0) return cluster_index;
            // Search for minimum value between its neighbors
            int min = N_MAX;
            for (int i = 1; i < N; i++) {
                if (neighbors[i] != -1) { // Check for negative array indices
                    // If the cluster index of the neighboring node is smaller than min (but non-negative), update minimum
                    int cl_n = nodes[neighbors[i]].cluster_index;
                    if (cl_n < min && cl_n != -1) min = cl_n;
                }
            }
            if (cluster_index != -1 && cluster_index < min) return cluster_index;
            else return min;
        }

};

// Function that prints all parameters of all the values in nodes array
void print_all(Node nodes[N_MAX]) {

    for (int n = 0; n < N; n++) nodes[n].print_params();
    return;

}

// Function that prints all parameters of all values in nodes array in file
void write_all(Node nodes[N_MAX]) {

    file = fopen("rnd_out.txt", "w");
    fprintf(file, "index\tx\ty\tz\tis_active\tcluster_index\n");
    for (int n = 0; n < N; n++) fprintf(file, "%02i\t%.5f\t%.5f\t%.5f\t%02i\t%02i\n", nodes[n].index, nodes[n].x, nodes[n].y, nodes[n].z, nodes[n].is_active, nodes[n].cluster_index);

    return;

}

// Function that prints all parameters of all values in nodes array in file
void write_all_init(Node nodes[N_MAX]) {

    file = fopen("rnd_out_init.txt", "w");
    fprintf(file, "index\tx\ty\tz\tis_active\tcluster_index\n");
    for (int n = 0; n < N; n++) fprintf(file, "%02i\t%.5f\t%.5f\t%.5f\t%02i\t%02i\n", nodes[n].index, nodes[n].x, nodes[n].y, nodes[n].z, nodes[n].is_active, nodes[n].cluster_index);

    return;

}

bool equal_arr(int a[N_MAX], int b[N_MAX]) {

    for (int i = 0; i < N_MAX; i++)
        if (a[i] != b[i]) return false;

    return true;

}

int main(void) {

    // Initialize random number generator
    extern gsl_rng *tau;
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau, SEED);

    // Array of all nodes (initialized to default values)
    Node *nodes;
    nodes = new Node[N_MAX];

    // Initialize all nodes
    for (int n = 0; n < N; n++) {
        // Set the positions (random between 0 and L)
        nodes[n].x = n%L;
        nodes[n].y = (-(n%L-n)/L)%L;
        nodes[n].z = -(n%(L*L)-n)/(L*L);
        // Set index of all nodes
        nodes[n].index = n;
        // Activate randomly some nodes with probability p0
        if (gsl_rng_uniform(tau) < P) 
            nodes[n].is_active = true;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Find neighbors at distance D of all nodes (each nodes object now contains array with neighboring indices)
    for (int n = 0; n < N; n++) nodes[n].find_neighbors(nodes, D);

    write_all_init(nodes);

    // Initialize cluster counter
    int cl = 0;
    // Scan over all nodes
    for (int n = 0; n < N; n++) {
        // Consider occupied nodes only
        if (nodes[n].is_active == true) {
            // At least one neighboring node labeled (Case c) -> Set cluster_index to minimum of the neighbors
            if (nodes[n].lab_neighbors(nodes))
                nodes[n].cluster_index = nodes[n].min_neighbor_label(nodes);
            // None of the neighboring nodes labeled (Case b) -> Begin new cluster, add node to it
            else
                cl++; nodes[n].cluster_index = cl;
        }
        // If node unoccupied, move on (Case a)
    }

    // Array that contains previous step (initialize with current cluster indices)
    int cl_array_p[N_MAX] = {-1};
    for (int i = 0; i < N_MAX; i++)
        cl_array_p[i] = nodes[i].cluster_index;
    // Array that contains current step (initially different from previous, so loop done at least once)
    int cl_array_c[N_MAX] = {0};
    // Max. number of iterations
    int n_steps = 10;
    int s = 0;
    // Check if the "solution" converges (finish once arrays are equal or after n_steps)
    while (equal_arr(cl_array_p, cl_array_c) == false || s <= n_steps ) {
        // Set both arrays to be equal
        for (int i = 0; i < N_MAX; i++)
            cl_array_p[i] = cl_array_c[i];
        // Go over all (active) nodes and set their cluster_index to the minimum value of their neighbors
        for (int n = 0; n < N; n++) {
            // Check if node is active
            if (nodes[n].is_active && nodes[n].min_neighbor_label(nodes) != -1) {
                // Find minimum cluster index between current node and neighbors
                int cl_min = std::min(nodes[n].cluster_index, nodes[n].min_neighbor_label(nodes));
                // Set cluster index of node to the minimum of its current value and the minimum value of its neighbors
                nodes[n].cluster_index = cl_min;
                for (int i = 0; i < N; i++)
                    if (nodes[n].neighbors[i] >= 0)
                        nodes[nodes[n].neighbors[i]].cluster_index = cl_min;
            }   
        }
        // Increase step counter
        s++;
        // Construct new current array
        for (int i = 0; i < N_MAX; i++)
            cl_array_c[i] = nodes[i].cluster_index;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    print_all(nodes);
    std::cout << "Computation time: " << duration.count() << " microsec" << std::endl;
    write_all(nodes);

    return 0;

}