#include<iostream>
#include<fstream>
#include<cmath>
#include<string>

#include<stdio.h>

#include"gsl_rng.h"

// Maximum size of arrays
#define N_MAX 1000

// Length of square inside which nodes are generated
int L = 10;
// Nodes to generate
int N = 500;
// Random seed
int SEED = 6857349;
// Initial probability
float P = 0.65;
// Distance threshold
float D = 1.5;
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
        // Array of indices of all neighbors (all -1 except index at position where neighbor is)
        int neighbors[N_MAX] = {-1};
        // Index of cluster node is in (default -1 cluster is none)
        int cluster_index = -1;
        // Number of active labeled neighboring nodes
        int n_labeled = 0;

        /* PRINTING DATA */

        // Function to output useful parameters
        void print_params() {

            printf("index=%02i\tx=%.5f\ty=%.5f\tis_active=%02i\tcluster_index=%02i\tn_labeled=%02i\n",index,x,y,is_active,cluster_index,n_labeled);
            return;

        }

        // Function to print all neighbor indices of the node
        void print_neighbors(Node nodes[N_MAX]) {
            // Run over all nodes
            for (int i = 0; i < N; i++)
                if (neighbors[i] >= 0) // Exclude auxiliary index values (-1)
                    printf("neighbor_index=%02i\tstatus=%02i\tcluster_index=%02i\n",neighbors[i], nodes[neighbors[i]].is_active, nodes[neighbors[i]].cluster_index);
            return;
        }

        /* CALCULATIONS */

        // Function to find (active) neighbors of node
        void find_neighbors(Node nodes[N_MAX]) {
            // Go over all nodes
            for (int i = 0; i < N; i++) {
                // Calculate distance
                float dist = sqrt( (nodes[i].x - x)*(nodes[i].x -x) + (nodes[i].y - y)*(nodes[i].y - y) );
                if (dist <= D && dist != 0 && nodes[i].is_active == true) // If distance is not zero (itself), less than threshold, and node active
                    neighbors[i] = nodes[i].index; // Add neighbors to neighbor array
                else neighbors[i] = -1;
            }
            return;
        }

        // Function to set all neighbors to the same cluster index
        void set_cluster_neighbors(Node nodes[N_MAX]) {
            for (int i = 0; i < N; i++) if (neighbors[i] >= 0) nodes[neighbors[i]].cluster_index = cluster_index;
            return;
        }

        // Function to count how many (active) labeled neighbors a node has
        int labeled_neighbors(Node nodes[N_MAX]) {

            for (int i = 0; i < N; i++)
                if (nodes[neighbors[i]].cluster_index != -1 && nodes[neighbors[i]].is_active == true) n_labeled += 1;
            return n_labeled;

        }

        // Function to obtain the minimum value of cluster_index of neighboring nodes
        int min_neighbor_label(Node nodes[N_MAX]) {
            // Set minimum value to cluster_index value of first neighbor (default -1)
            int min = N_MAX+1;
            for (int i = 1; i < N; i++) {

                if(nodes[neighbors[i]].cluster_index < min && nodes[neighbors[i]].cluster_index != -1 && neighbors[i] != -1) min = nodes[neighbors[i]].cluster_index;

            }

            if (min != N_MAX+1) return min;
            else return -1;

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
    fprintf(file, "index\tx\ty\tis_active\tcluster_index\n");
    for (int n = 0; n < N; n++) fprintf(file, "%02i\t%.5f\t%.5f\t%i\t%i\n",nodes[n].index,nodes[n].x,nodes[n].y,nodes[n].is_active,nodes[n].cluster_index);

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
    //Cluster clusters[N];

    // Initialize all nodes
    for (int n = 0; n < N; n++) {
        // Set the positions (random between 0 and L)
        nodes[n].x = gsl_rng_uniform(tau)*L;
        nodes[n].y = gsl_rng_uniform(tau)*L;
        // Set index of all nodes
        nodes[n].index = n;
        // Activate randomly some nodes with probability p0
        if (gsl_rng_uniform(tau) < P) 
            nodes[n].is_active = true;
    }

    // Find neighbors of all nodes (each nodes object now contains array with neighboring indices)
    for (int n = 0; n < N; n++) nodes[n].find_neighbors(nodes);

    // Initialize cluster counter
    int c = 0;
    // Scan over all nodes
    for (int n = 0; n < N; n++) {
        // "Occupied" node
        if (nodes[n].is_active == true) {
            // If at least one neighboring node is labeled (Case c)
            if (nodes[n].labeled_neighbors(nodes) > 0) {
                // Set cluster_index to minimum of the neighbors
                nodes[n].cluster_index = nodes[n].min_neighbor_label(nodes);
            // If none of the neighboring nodes are labeled (Case b)
            } else if (nodes[n].labeled_neighbors(nodes) <= 0 || nodes[n].labeled_neighbors(nodes) == N_MAX+1) { 
                // Begin new cluster
                c += 1;
                // Add node to cluster
                nodes[n].cluster_index = c;
            }
        }
        // If current node is "unoccupied" simply move on (Case a)
    }

    // Go over all (active) nodes and set their cluster_index to the minimum value of their neighbors
    for (int n = 0; n < N; n++) {

        if (nodes[n].is_active == true) {

            nodes[n].cluster_index = std::min(nodes[n].cluster_index, nodes[n].min_neighbor_label(nodes));
            // Also set cluster_index of neighbors to this same minimum
            for (int i = 0; i < N; i++) {
                nodes[nodes[n].neighbors[i]].cluster_index = std::min(nodes[nodes[n].neighbors[i]].cluster_index,nodes[n].cluster_index);
            }

        }

    }

    print_all(nodes);

    write_all(nodes);

    return 0;

}