// Include library with all classes necessary
#include"percolation.hpp"
using namespace std;

// Maximum size of arrays (check so it is the same as in percolation.hpp!)
#define N_MAX 1000000

// Nodes to generate
int N = 25;
// Length of square inside which nodes are generated
int L = 25;
// Initial probability
float P = 0.40;
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
    fprintf(file, "%i\t%.5f\n", L, P); // Print lattice stats
    fprintf(file, "%.3f\n", D); // Print distance threshold used to find neighbors
    // Print index, position, is_active, cluster_index and n_neighbors
    fprintf(file, "index\tx\ty\tis_active\tcluster_index\tn_neighbors\n");
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

    //for (int p = 0.00; p <= 1.00; p = p + 0.01) {

    for (int p = 0.5; p <= 0.5; p++) {

    // Create the lattice
    Lattice lattice(N, L);

    // Generate a randomly distributed lattice
    lattice.generate("sqr", P);
    // Display lattice on screen
    lattice.display_lattice();
    // Save lattice to file
    lattice.write_lattice(L, P);

    }

    /* INITIALIZATION */

    // Initialize input and output files
    FILE *input, *output;

    // Array of all nodes (initialized to default values)
    Node *nodes;
    nodes = new Node[N_MAX];

    /* READ FROM FILE */

    // Input file name "lattice.txt", fmt: "index \t x \t y \t is_active \t cluster_index \n"
    input = fopen("lattice.txt", "r");
    fscanf(input, "%i\t%f\n", &L, &P); // Read lattice stats
    fscanf(input, "%*[^\n]\n"); // Skip second row
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
    for (int n = 0; n < n_count; n++) nodes[n].find_neighbors(nodes, D);

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

    // Input file name "lattice_sorted.txt", fmt: "index \t x \t y \t is_active \t cluster_index \n"
    input = fopen("lattice_sorted.txt", "r");
    fscanf(input, "%i\t%f\n", &L, &P); // Read L and P values
    fscanf(input, "%*[^\n]\n"); // Skip row
    fscanf(input, "%*[^\n]\n"); // Skip row
    // Count how many  rows (nodes) we have in the file
    for (int i = 0; i < N_MAX; i++) {
        int temp;
        // Read all values (5 for each row) until end of file or any kind of error (return != 6)
        if ( fscanf(input, "%i\t%f\t%f\t%i\t%i\t%i", &nodes[i].index, &nodes[i].x, &nodes[i].y, &temp, &nodes[i].cluster_index, &nodes[i].n_neighbors) == 6) {
            n_count = i+1;
            nodes[i].is_active = temp;
        }
        else i = N_MAX;
    }
    // Close the file
    fclose(input);

    int actives = 0;
    for (int j = 0; j < n_count; j++){
        if(nodes[j].is_active){
        actives++;
        //cout << nodes[j].n_neighbors << "\n";
        }
    }

                // max = number of clusters
    int max = 0;
    for(int i = 0; i < n_count; i++)
        if(nodes[i].cluster_index > max)
            max = nodes[i].cluster_index;


    cout << "Number of Clusters: " << max << "\n";
    cout << "Total number of nodes: " << n_count << "\n";
    cout << "Number of active nodes: " << actives << "\n";
    cout << "que" << "\n";

    //cluster cluster[max];
    Cluster *cluster;
    cluster = new Cluster[max];

    cout << "que" << "\n";

                //Set the index of all the clusters
    for(int j = 0; j < max; j++)
        cluster[j].Index = j+1;
    
    for(int j =0; j < max; j++){
        int cont = 0;
        for(int i = 0; i < n_count; i++){
            if((nodes[i].is_active == true)&&(nodes[i].cluster_index == cluster[j].Index)){
                cont++;
            }
            if(i == n_count - 1)            //Number of elements of the cluster
                cluster[j].N = cont;
        }
        //cout << "Cluster: " << j << " Numb of nodes: " << cluster[j].N << "\n";
    }

                //Fill the elements of the clusters
    for(int j =0; j < max; j++){
        int cont = 0;
        for(int i = 0; i < n_count; i++){
            if((nodes[i].is_active == true)&&(nodes[i].cluster_index == cluster[j].Index)){
                
                //cout << "Comprobaciones:" << "\n";
                //cout << nodes[i].cluster_index << "\n";
                //cout << cluster[j].GreatN << "\n";
                //cout << cluster[j].getNode(cont).x << "\n";
                cluster[j].getNode(cont).cluster_index = nodes[i].cluster_index;
                cluster[j].getNode(cont).x = nodes[i].x;
                cluster[j].getNode(cont).y = nodes[i].y;
                cluster[j].getNode(cont).is_active = nodes[i].is_active;
                cluster[j].getNode(cont).n_neighbors = nodes[i].n_neighbors;
                //cout << cluster[j].getNode(cont).y << endl;
                //cout << j << "\n";
                //cout << cont << "\n";
                cont++;
            }
        }
    }

    for(int j = 0; j < max; j++){
        
        double xmin = 10000;
        double xmax = 0;
        double ymin = 10000;
        double ymax = 0;
        for(int i = 0; i < cluster[j].N; i++){          //Defines the up, down, left & right limits of the cluster  
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

        /*
        cout << "For cluster " << j << " that has " << cluster[j].N << " nodes: Xmax= " << xmax << ", Xmin= " << xmin << ", Ymax= " << ymax << ", Ymin= " << ymin << "\n";
        cout << "One element of the cluster is x=" << cluster[j].getNode(0).x << " y=" << cluster[j].getNode(1).y << "\n";
        cout << "Scale= " << cluster[j].scale << "\n";
        cout << "\n";
        */
    }


    for(int j = 0; j < max; j++){
        cluster[j].perimeter = 0;
        for(int i = 0; i < cluster[j].N; i++)
            cluster[j].perimeter = cluster[j].perimeter + 4 -cluster[j].getNode(i).n_neighbors;
    }

    //for(int j = 0; j < max; j++)
    //    cout << "Cluster: " << j << " Nodes:" << cluster[j].N << " Scale: " << cluster[j].scale << " Perimeter: " << cluster[j].perimeter << "\n";

    char filename[100]; // Asegúrate de que el tamaño sea suficiente para contener el nombre del archivo
    sprintf(filename, "cluster_data_(L=%i, P=%f).txt", L, P);
    
    FILE *file;
    file = fopen(filename, "w");
    fprintf(file, "cluster_index\tscale\tperimeter\n");
    for (int n = 0; n < max; n++)
        if(cluster[n].N != 0)
            fprintf(file, "%i\t%i\t%i\n", cluster[n].Index, cluster[n].scale, cluster[n].perimeter);
    fclose(file);

    // Read from file cluster size and print on scren (or file): prob_perc, avg_csize

    return 0;

}