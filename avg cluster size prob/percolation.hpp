#pragma once

#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#include<chrono>
#include<iostream>

#include<vector>

#include"gsl_rng.h"

#define N_MAX 1000000

// Random variable pointer
gsl_rng *mu;

// NODE CLASS
class Node {

    public:

        /* PARAMETERS */

        // Position of node
        float x, y;
        // Active-or-not status
        bool is_active;
        // Node index and cluster index it belongs to
        int index, cluster_index;
        
        // Vector of neighbor indices (initially empty)
        std::vector<int> neighbors = std::vector<int>(0);
        
        // Number of active neighbors of node
        int n_neighbors;

        // Number of nodes in lattice
        int N;

        // Default constructor
        Node() {
            x = 0;
            y = 0;
            is_active = false;
            index = 0;
            cluster_index = -1;
            n_neighbors = 0;
        }

        // PRINTING DATA //

        // Function to output useful parameters of the node
        void print_params() {
            printf("index=%02i\tx=%.5f\ty=%.5f\tis_active=%02i\tcluster_index=%02i\n",index,x,y,is_active,cluster_index);
            return;
        }

        // Function to print all neighboring indices (and some useful parameters) of the node
        void print_neighbors(Node nodes[N_MAX]) {
            // Run over all nodes
            for (int i : neighbors) printf("neighbor_index=%02i\tcluster_index=%02i\n", i, nodes[i].cluster_index);
            return;
        }

        // CALCULATIONS //

        // Function to find (active) neighbors of node and count them
        void find_neighbors(Node nodes[N_MAX], float D) {
            // If current node is inactive, return
            if (is_active == false) return;
            // Run over all nodes in the system
            for (int i = 0; i < N; i++) {
                // Calculate distance between current node and all others
                float dist = sqrt( (nodes[i].x - x)*(nodes[i].x -x) + (nodes[i].y - y)*(nodes[i].y - y) );
                // If the distance is small (non-zero) and the other node is active
                if (dist <= D && dist != 0 && nodes[i].is_active == true)
                    neighbors.push_back(nodes[i].index); // Append neighbor index to array
                // If the distance if far away or other node inactive, ignore
            }
            // Count the number of active neighbors found
            n_neighbors = neighbors.size();
            return;
        }

        // Function to find whether any (at least one) neighbor of current node is labeled
        bool lab_neighbors(Node nodes[N_MAX]) {
            for (int i : neighbors) if (nodes[i].cluster_index != -1) return true;
            return false;
        }

        // Function to set all neighbors to the same cluster index (the minimum value between current node and neighbors)
        void set_cluster_neighbors(Node nodes[N_MAX]) {
            if (n_neighbors != 0)
                for (int i : neighbors) {
                    int cl_min = std::min(cluster_index, nodes[i].cluster_index);
                    nodes[i].cluster_index = cl_min;
                    cluster_index = cl_min;
                }
            return;
        }

        // Function to obtain the minimum value of cluster_index of neighboring nodes
        int min_neighbor_label(Node nodes[N_MAX]) {
            // If the node has no neighbors, return his own cluster index
            if (n_neighbors == 0) return cluster_index;
            // Search for minimum value between its neighbors
            int min = N_MAX;
            for (int i : neighbors) {
                int cl_n = nodes[i].cluster_index;
                if (cl_n < min && cl_n != -1) min = cl_n;
            }
            if (cluster_index != -1 && cluster_index < min) return cluster_index;
            else return min;
        }

        // Function that checks whether two arrays are equal (all their elements are equal)
        static bool equal_arr(int a[N_MAX], int b[N_MAX]) {
            for (int i = 0; i < N_MAX; i++)
                if (a[i] != b[i]) return false;
            return true;
        }

};

class Lattice {

    public:

        // Type of lattice (square = "sqr", triangular = "tri", random = "rnd")
        std::string type;
        // Number of nodes to generate
        int n_nodes;
        // Length of square inside which nodes are generated
        int length;
        // Probability of a node being active
        float prob;

        // Create default node array (pointer to)
        Node *nodes = new Node[N_MAX];
        
        // Constructor
        Lattice(int N, int L) {

            // Initialize indices of nodes
            for (int i = 0; i < N_MAX; i++) nodes[i].index = i;

            // Set number of nodes and length of square 
            n_nodes = N;
            length = L;

            return;

        }

        // Generate lattice
        void generate(std::string type, float prob) {

            // Random seed
            int SEED = time(NULL);
            // Initialize random number generator
            extern gsl_rng *mu;
            mu = gsl_rng_alloc(gsl_rng_taus);
            gsl_rng_set(mu, SEED);

            // Square lattice (begins at point (1,1))
            if (type == "sqr") {

                // Check if the dimensions are correct
                if (length*length != n_nodes)
                    // Prioritize length parameter and make number of nodes match
                    n_nodes = length*length;

                for (int n = 0; n < n_nodes; n++)  {
                    nodes[n].x = n%length;
                    nodes[n].y = -(n%length-n)/length;
                    if (gsl_rng_uniform(mu) < prob)
                        nodes[n].is_active = true;
                }

            }
            

            // Triangular lattice (to implement...)
            else if (type == "tri") {

                // Check if the dimensions are correct
                if (length*length != n_nodes)
                    // Prioritize length parameter and make number of nodes match
                    n_nodes = length*length;

                for (int n = 0; n < n_nodes; n++)  {
                    // Horizontal position depends on whether vertical position is even or odd
                    int y = -(n%length-n)/length;
                    if (y%2 == 0) // Even row
                        nodes[n].x = n%length;
                    else // Odd row
                        nodes[n].x = n%length-0.5;
                    nodes[n].y = y; // Vertical position is same as square lattice
                    if (gsl_rng_uniform(mu) < prob)
                        nodes[n].is_active = true;
                }

            }

            // In any other case print random lattice
            else {

                for (int i = 0; i < n_nodes; i++) {

                    nodes[i].x = gsl_rng_uniform(mu)*length;
                    nodes[i].y = gsl_rng_uniform(mu)*length;
                    if (gsl_rng_uniform(mu) < prob) 
                        nodes[i].is_active = true;

                }

            }

            return;

        }

        // Print lattice in terminal
        void display_lattice() {

            for (int i = 0; i < n_nodes; i++)
                printf("index=%02i\tx=%.5f\ty=%.5f\tis_active=%i\tcluster_index=%02i\n",nodes[i].index,nodes[i].x,nodes[i].y,nodes[i].is_active,nodes[i].cluster_index);

            return;

        }

        // Write lattice to file
        void write_lattice(int L, float P) {

            FILE *file;
            file = fopen("lattice.txt", "w");
            // Print stats of lattice
            fprintf(file,"%i\t%f\n", L, P);
            // Print index, position, is_active and cluster_index
            fprintf(file,"index\tx\ty\tis_active\tcluster_index\n");
            for (int i = 0; i < n_nodes; i++)
                fprintf(file,"%i\t%.5f\t%.5f\t%i\t%i\n",nodes[i].index,nodes[i].x,nodes[i].y,nodes[i].is_active,nodes[i].cluster_index);

            return;

        }

};

class Cluster{

    public:
    
    // Number of elements
    int N;
    // Index
    int Index;
    // Characteristic scale of the cluster
    int scale;
    // Perimeter of the cluster
    int perimeter;    
    // Number of nodes in the cluster
    int n_nodes;

    // Elements of the cluster
    std::vector<Node> tree = std::vector<Node>(0);  

    //Constructor
    Cluster() {

        N=0; 
        Index=0; 
        scale=0; 
        perimeter=0;

    } 

    //Access the element 'pos' of the cluster
    Node& getNode(int pos) {
        return tree[pos];

    }

    void get_n_nodes() {

        n_nodes = std::size(tree);

    }
    
};