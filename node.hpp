#pragma once

#include<math.h>
#include<stdio.h>

#define N_MAX 10000

class Node {

    public:

        /* PARAMETERS */

        // Position of node
        float x, y;
        // Active-or-not status
        bool is_active;
        // Node index and cluster index it belongs to
        int index, cluster_index;
        // Array of indices of all neighbors (all -1 except index at position where neighbor is)
        int neighbors[N_MAX] = {-1};
        // Number of active neighbors of node
        int n_neighbors;

        int N = 25;

        // Default constructor
        Node() {
            x = 0;
            y = 0;
            is_active = false;
            index = 0;
            cluster_index = -1;
            n_neighbors = 0;
            for (int i = 0; i < N_MAX; i++) neighbors[i] = -1;
        }

        /* PRINTING DATA */

        // Function to output useful parameters of the node
        void print_params() {
            printf("index=%02i\tx=%.5f\ty=%.5f\tis_active=%02i\tcluster_index=%02i\n",index,x,y,is_active,cluster_index);
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
        void find_neighbors(Node nodes[N_MAX], float D) {
            // If current node is inactive, return
            if (is_active == false) return;
            // Run over all nodes in the system
            for (int i = 0; i < N; i++) {
                // Calculate distance between current node and all others
                float dist = sqrt( (nodes[i].x - x)*(nodes[i].x -x) + (nodes[i].y - y)*(nodes[i].y - y) );
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