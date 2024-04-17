/* 

    CODE USED TO SORT NODES IN A LATTICE INTO CLUSTERS
    INPUTS: "lattice.txt" -> Text file containing parameters of all nodes
    OUTPUTS: "lattice_sorted.txt" -> Text file containing the same parameters sorted by cluster index

*/

#include<iostream>
#include<chrono>

#include"node.hpp" // Include the node library that contains all useful functions

// Maximum size of arrays
#define N_MAX 1000

// Nodes to print (later on specified by n_count)
int N;

// Distance-to-neighbor threshold
float D = 1.25;

// Function that prints all parameters of all the values in nodes array
void print_all(Node nodes[N_MAX]) {
    for (int n = 0; n < N; n++) nodes[n].print_params();
    return;
}

// Function that prints all parameters of all values in nodes array in file
void write_all(Node nodes[N_MAX]) {
    FILE *file;
    file = fopen("lattice_sorted.txt", "w");
    fprintf(file, "index\tx\ty\tis_active\tcluster_index\tn_neighbors\n");
    fprintf(file, "%.3f\t0\t0\t0\t0\t0\n", D); // Print distance threshold used to find neighbors
    for (int n = 0; n < N; n++) fprintf(file, "%i\t%.5f\t%.5f\t%i\t%i\t%i\n", nodes[n].index, nodes[n].x, nodes[n].y, nodes[n].is_active, nodes[n].cluster_index, nodes[n].n_neighbors);
    fclose(file);
    return;
}

// Function that checks whether two arrays are equal (all their elements are equal)
bool equal_arr(int a[N_MAX], int b[N_MAX]) {
    for (int i = 0; i < N_MAX; i++)
        if (a[i] != b[i]) return false;
    return true;
}

int main(void) {

    /* INITIALIZATION */

    // Initialize input and output files
    FILE *input, *output;

    // Array of all nodes (initialized to default values)
    Node *nodes;
    nodes = new Node[N_MAX];

    /* READ FROM FILE */

    // Input file name "lattice.txt", fmt: "index \t x \t y \t is_active \t cluster_index \n"
    input = fopen("lattice.txt", "r");
    fscanf(input, "%*[^\n]\n"); // Skip first row
    // Count how many rows (nodes) we have in the file
    int n_count = 0;
    for (int i = 0; i < N_MAX; i++) {
        int is_active = 0;
        // Read all values (5 for each row) until end of file or any kind of error (return != 5)
        if ( fscanf(input, "%i\t%f\t%f\t%i\t%i", &nodes[i].index, &nodes[i].x, &nodes[i].y, &is_active, &nodes[i].cluster_index) == 5) {
            nodes[i].is_active = is_active;
            n_count = i+1;
        }
        else i = N_MAX;
    }
    // Close the file
    fclose(input);

    for (int i = 0; i < n_count; i++)
        nodes[i].N = n_count;
    N = n_count;

    // Just to check if it reads correctly (watch out for value N to show and N_MAX maximum lines read!)
    std::cout << "INITIAL STATE" << std::endl;
    print_all(nodes);

    auto start = std::chrono::high_resolution_clock::now(); // "Stopwatch"

    // Find neighbors of all nodes (each nodes object now contains array with neighboring indices)
    for (int n = 0; n < N; n++) nodes[n].find_neighbors(nodes, D);

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
    int n_steps = 50;
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

    // Stopwatch
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // Print final state on screen
    std::cout << "FINAL STATE" << std::endl;
    print_all(nodes);

    // Display computation time
    std::cout << "Computation time: " << duration.count() << " microsec" << std::endl;

    // Write final state into file
    write_all(nodes);

    return 0;

}