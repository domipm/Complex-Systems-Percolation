// Include library with all classes necessary
#include"percolation.hpp"

#include"gsl_randist.h"

// Length of square inside which nodes are generated (bounds on forest)
int L = 500;
// Number of nodes to generate (density)
int N = 0.1475 * L * L;
// Initial probability
float P = 1;

// High-distance
float d_big = 5;
// Low distance
float d_small = 3;

// Ratio of high-distance / low-distance trees
float ratio = 0.05; // 1 = Only high-distance trees

// Function that checks whether two arrays are equal (all their elements are equal)
bool equal_arr(int a[N_MAX], int b[N_MAX]) {
    for (int i = 0; i < N_MAX; i++)
        if (a[i] != b[i]) return false;
    return true;
}

// Random variable pointer
gsl_rng *nu;

int main(void) {

    // Random seed
    //int SEED = time(NULL);
    int SEED = 314159265;
    // Initialize random number generator
    extern gsl_rng *nu;
    nu = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(nu, SEED);

    int n_count = N; // Number of nodes in random lattice

    // Create the lattice
    Lattice lattice(N, L);
    // Array of all nodes (initialized to default values)
    Node *nodes;
    nodes = new Node[N_MAX];
    //cluster cluster[max];
    Cluster *cluster;

    FILE *file;
    char filename[100];
    sprintf(filename, "loop_%i_%i_%.2f.txt", L, N, ratio);
    file = fopen(filename, "w");
    fprintf(file, "probability\taverage cluster size\n");

    /* LATTICE GENERATOR */

    // Generate a square lattice (activate all nodes)
    lattice.generate("rnd", 1);

    // Set neighbor threshold distance of nodes according to tree ratio
    for (int i = 0; i < n_count; i++)
        if (gsl_rng_uniform(nu) < ratio)
            nodes[i].D = d_big;
        else
            nodes[i].D = d_small;

    // Loop over probabilities
    //for (double k = 0.00; k < 1.00; k = k + 0.05) {

        //double p = k;

    for (double k = 0.00; k < 1.00; k = k + 0.01) {

        double p = k;

        std::cout << "Probability " << p << std::endl;

        // Restart lattice nodes active status
        for (int n = 0; n < n_count; n++)
            lattice.nodes[n].is_active = false;

        // Activate or not nodes in lattice with probability p (of loop)
        for (int n = 0; n < n_count; n++)
            if (gsl_rng_uniform(nu) < p)
                lattice.nodes[n].is_active = true;

        /* HOSHEN-KOPELMAN */

        for (int i = 0; i < n_count; i++) {

            nodes[i].x = lattice.nodes[i].x;
            nodes[i].y = lattice.nodes[i].y;
            nodes[i].is_active = lattice.nodes[i].is_active;
            nodes[i].index = lattice.nodes[i].index;
            nodes[i].n_neighbors = lattice.nodes[i].n_neighbors;
            nodes[i].cluster_index = lattice.nodes[i].cluster_index;

        }

        for (int i = 0; i < n_count; i++) nodes[i].N = n_count;

        // Print initial lattice
        /*
        FILE *file;
        char filename[100];
        sprintf(filename, "init_%i_%i_%.2f_%.2f.txt", L, N, ratio, P);
        file = fopen(filename, "w");
        // Print index, position, is_active and cluster_index
        fprintf(file,"index\tx\ty\tis_active\tcluster_index\tdistance\n");
        for (int i = 0; i < n_count; i++)
            fprintf(file,"%i\t%.5f\t%.5f\t%i\t%i\t%.2f\n",nodes[i].index,nodes[i].x,nodes[i].y,nodes[i].is_active,nodes[i].cluster_index, nodes[i].D);
        fclose(file);
        */

        // Find neighbors of all nodes (each nodes object now contains array with neighboring indices)
        for (int n = 0; n < n_count; n++) nodes[n].find_neighbors(nodes);

        // Initialize cluster counter
        int cl = 0;
        // Scan over all nodes
        for (int n = 0; n < n_count; n++) {
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
        int n_steps = 100;
        int s = 0;
        // Check if the "solution" converges (finish once arrays are equal or after n_steps)
        while (equal_arr(cl_array_p, cl_array_c) == false || s <= n_steps ) {
            // Set both arrays to be equal
            for (int i = 0; i < N_MAX; i++)
                cl_array_p[i] = cl_array_c[i];
            // Go over all (active) nodes and set their cluster_index to the minimum value of their neighbors
            for (int n = 0; n < L*L; n++) {
                // Check if node is active
                if (nodes[n].is_active && nodes[n].min_neighbor_label(nodes) != -1) {
                    // Find minimum cluster index between current node and neighbors
                    int cl_min = std::min(nodes[n].cluster_index, nodes[n].min_neighbor_label(nodes));
                    // Set cluster index of node to the minimum of its current value and the minimum value of its neighbors
                    nodes[n].cluster_index = cl_min;
                    // For nodes[n] go over all its neighbors
                    for (int i = 0; i < nodes[n].neighbors.size(); i++)
                        nodes[nodes[n].neighbors.at(i)].cluster_index = cl_min;
                }   
            }
            // Increase step counter
            s++;
            // Construct new current array
            for (int i = 0; i < N_MAX; i++)
            cl_array_c[i] = nodes[i].cluster_index;
        }

        /* CLUSTER STATS */

        int actives = 0;
        for (int j = 0; j < n_count; j++){
            if(nodes[j].is_active)
                actives++;
        }

        int max = 0;
        for(int i = 0; i < n_count; i++)
            if(nodes[i].cluster_index > max)
                max = nodes[i].cluster_index;

        cluster = new Cluster[max];

        std::cout << "Maximum index of cluster: " << max << "\n";
        std::cout << "Total number of nodes: " << n_count << "\n";
        std::cout << "Number of active nodes: " << actives << "\n";

        //Set the index of all the clusters
        for(int j = 0; j < max; j++)
            cluster[j].Index = j+1;

        for(int j =0; j < max; j++){
            int cont = 0;
            for(int i = 0; i < n_count; i++){
                if((nodes[i].is_active == true)&&(nodes[i].cluster_index == cluster[j].Index))
                    cont++;
                if(i == n_count - 1) cluster[j].N = cont;
            }
        }

        //Fill in the elements of the clusters
        for(int j =0; j < max; j++)
            for(int i = 0; i < n_count; i++)
                if((nodes[i].is_active == true)&&(nodes[i].cluster_index == cluster[j].Index))  
                    cluster[j].tree.push_back(nodes[i]); 

        for(int j = 0; j < max; j++) {

            double xmin = 10000;
            double xmax = 0;
            double ymin = 10000;
            double ymax = 0;
            for(int i = 0; i < cluster[j].N; i++) {    
                if(cluster[j].getNode(i).x < xmin)
                    xmin = cluster[j].getNode(i).x;
                if(cluster[j].getNode(i).x > xmax)
                    xmax = cluster[j].getNode(i).x;
                if(cluster[j].getNode(i).y < ymin)
                    ymin = cluster[j].getNode(i).y;
                if(cluster[j].getNode(i).y > ymax)
                    ymax = cluster[j].getNode(i).y;        
            }

            if(cluster[j].N == 0)
                cluster[j].scale = 0;
            else if(cluster[j].N == 1)
                cluster[j].scale = 1;
            else if((xmax - xmin)>(ymax - ymin))
                cluster[j].scale = (xmax - xmin) +1;
            else
                cluster[j].scale = (ymax - ymin) +1;

        }

        for(int j = 0; j < max; j++){
            cluster[j].perimeter = 0;
            for(int i = 0; i < cluster[j].N; i++)
                cluster[j].perimeter = cluster[j].perimeter + 4 - cluster[j].getNode(i).n_neighbors;
        }

        // Calculate the number of nodes in each cluster
        for (int n = 0; n < max; n++) cluster[n].get_n_nodes();

        // Check if percolated
        for (int i = 0; i < max; i++)
            if (cluster[i].scale >= L)
                std::cout << "System has percolated!" << std::endl;

        // Compute average cluster size
        float avg_csize = 0.0;
        float n_clusters = 0;
        for (int i = 0; i < max; i++) {
            if (cluster[i].n_nodes != 0 && cluster[i].scale < L) {

                avg_csize += cluster[i].n_nodes;
                n_clusters++;

            }
        }

        // Display average cluste size
        avg_csize = avg_csize/n_clusters;
        std::cout << "Average cluster size: " << avg_csize << std::endl;

        // Output number of nodes in each cluster into file
        /*
        for (int i = 0; i < max; i++)
            if (cluster[i].n_nodes != 0)
                fprintf(file, "%i\t%i\n", cluster[i].Index, cluster[i].n_nodes);

        fclose(file);
        */

        fprintf(file, "%f\t%f\n", p, avg_csize);

    }

    fclose(file);

    /*
    char filenamef[100];
    sprintf(filenamef, "final_%i_%i_%.2f_%.2f.txt", L, N, ratio, P);
    file = fopen(filenamef, "w");
    // Print index, position, is_active and cluster_index
    fprintf(file,"index\tx\ty\tis_active\tcluster_index\tdistance\n");
    for (int i = 0; i < n_count; i++)
        fprintf(file,"%i\t%.5f\t%.5f\t%i\t%i\t%.2f\n",nodes[i].index,nodes[i].x,nodes[i].y,nodes[i].is_active,nodes[i].cluster_index, nodes[i].D);
    fclose(file);
    */

    return 0;

}