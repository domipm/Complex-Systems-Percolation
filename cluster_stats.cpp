/* 

    CODE USED TO OBTAIN THE DATA OF ALL CLUSTERS
    INPUTS: "lattice_sorted.txt" -> Obtained by "hoshen-kopelman.cpp" algorithm that sorts nodes into clusters
    OUTPUTS: "cluster_data.txt" -> Text file will each cluster and its scale and perimeter

*/

#include"percolation.hpp"

using namespace std;

int main(void){

    
    // Array of all nodes (initialized to default values)
    Node *nodes;
    nodes = new Node[N_MAX];
    FILE *input;

    int L; // Length of square
    float P; // Probability of spawn

    // Input file name "lattice_sorted.txt", fmt: "index \t x \t y \t is_active \t cluster_index \n"
    input = fopen("lattice_sorted.txt", "r");
    fscanf(input, "%i\t%f\n", &L, &P); // Read L and P values
    fscanf(input, "%*[^\n]\n"); // Skip row
    fscanf(input, "%*[^\n]\n"); // Skip row
    // Count how many  rows (nodes) we have in the file
    int n_count = 0;
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
    
    

                                //Fill in the elements of the clusters
    for(int j =0; j < max; j++){
        //int cont = 0;
        for(int i = 0; i < n_count; i++){
            if((nodes[i].is_active == true)&&(nodes[i].cluster_index == cluster[j].Index)){
                
                //cout << "Comprobaciones:" << "\n";
    /*
                cluster[j].getNode(cont).cluster_index = nodes[i].cluster_index;
                cluster[j].getNode(cont).x = nodes[i].x;
                cluster[j].getNode(cont).y = nodes[i].y;
                cluster[j].getNode(cont).is_active = nodes[i].is_active;
                cluster[j].getNode(cont).n_neighbors = nodes[i].n_neighbors;
    */            
                cluster[j].tree.push_back(nodes[i]); 

                //cout << cont << "\n";
                //cont++;
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

return 0;

}
