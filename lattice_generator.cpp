/* 

    CODE USED TO GENERATE THE (LATTICE OR NON-LATTICE) SYSTEM OF NODES
    OUTPUTS: "lattice.txt" -> Text file with the positions and activity status of all nodes
                              (by default, all nodes have cluster_index = -1)

*/

#include"lattice.hpp" // Include lattice library that contains all useful functions

// Maximum size of arrays
#define N_MAX 1000

// Nodes to generate
int N = 50;
// Length of square inside which nodes are generated
int L = 7;
// Initial probability
float P = 0.35;

int main(void) {

    // Create the lattice
    Lattice lattice(N, L);

    // Generate a randomly distributed lattice
    lattice.generate("sqr", P);
    // Display lattice on screen
    lattice.display_lattice();
    // Save lattice to file
    lattice.write_lattice();

    return 0;

}