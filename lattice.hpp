#pragma once

#include<stdio.h> // Used for printing and string variable
#include<iostream>
#include<time.h>
#include<cmath>

#include"gsl_rng.h" // Used for random number generation

#include"node.hpp" // Include so we can create nodes

#define N_MAX 1000

// Random variable pointer
gsl_rng *mu;

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
        Node *nodes;
        
        // Constructor
        Lattice(int N, int L) {

            // Node array (set to default values)
            nodes = new Node[N_MAX];

            // Initialize indices of nodes
            for (int i = 0; i < N_MAX; i++)
                nodes[i].index = i;

            // Set number of nodes, length of square and probability of being active
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

                printf("SQUARE LATTICE\n");

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

                printf("tri\n");

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

            else if (type == "rnd") {

                printf("RANDOM LATTICE\n");

                for (int i = 0; i < n_nodes; i++) {

                    nodes[i].x = gsl_rng_uniform(mu)*length;
                    nodes[i].y = gsl_rng_uniform(mu)*length;
                    if (gsl_rng_uniform(mu) < prob) 
                        nodes[i].is_active = true;

                }

            }

            else {

                printf("ERROR: Lattice type not recognized!\n");
                return;

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
        void write_lattice() {

            FILE *file;
            file = fopen("lattice.txt", "w");

            fprintf(file,"index\tx\ty\tis_active\tcluster_index\n");
            for (int i = 0; i < n_nodes; i++)
                fprintf(file,"%i\t%.5f\t%.5f\t%i\t%i\n",nodes[i].index,nodes[i].x,nodes[i].y,nodes[i].is_active,nodes[i].cluster_index);

            return;

        }


};