#include<iostream>
#include<math.h>
#include<stdio.h>
#include "node.hpp"
#include "cluster.hpp"
using namespace std;


int main(void){

    
    // Array of all nodes (initialized to default values)
    Node *nodes;
    nodes = new Node[N_MAX];
    FILE *input;
    

    // Input file name "lattice_sorted.txt", fmt: "index \t x \t y \t is_active \t cluster_index \n"
    input = fopen("lattice_sorted.txt", "r");
    fscanf(input, "%*[^\n]\n"); // Skip first row
    fscanf(input, "%*[^\n]\n");
    // Count how many  rows (nodes) we have in the file
    int n_count = 0;
    for (int i = 0; i < N_MAX; i++) {
        int temp;
        // Read all values (5 for each row) until end of file or any kind of error (return != 5)
        if ( fscanf(input, "%i\t%f\t%f\t%i\t%i", &nodes[i].index, &nodes[i].x, &nodes[i].y, &temp, &nodes[i].cluster_index) == 5) {
            n_count = i+1;
            nodes[i].is_active = temp;
        }
        else i = N_MAX;
    }
    // Close the file
    fclose(input);

    int actives = 0;
    for (int j = 0; j < n_count; j++){
        if(nodes[j].is_active)
        actives++;
    }
    

                // max = number of clusters
    int max = 0;
    for(int i = 0; i < n_count; i++)
        if(nodes[i].cluster_index > max)
            max = nodes[i].cluster_index;


    cout << max << "\n";
    cout << n_count << "\n";
    cout << actives << "\n";


    //cluster cluster[max];
    Cluster *cluster;
    cluster = new Cluster[max];

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
        cout << "Cluster: " << j << " Numb of nodes: " << cluster[j].N << "\n";
    }
    
    
                //Fill the elements of the clusters
    for(int j =0; j < max; j++){
        int cont = 0;
        for(int i = 0; i < n_count; i++){
            if((nodes[i].is_active == true)&&(nodes[i].cluster_index == cluster[j].Index)){
                
                //cout << "Comprobaciones:" << "\n";
                //cout << cluster[j].N << "\n";
                //cout << nodes[i].cluster_index << "\n";
                //cout << cluster[j].tree[cont].cluster_index;
                //cout << cluster[j].GreatN << "\n";
                //cout << cluster[j].getNode(cont).x << "\n";
                cluster[j].getNode(cont).x = nodes[i].x;
                cluster[j].getNode(cont).y = nodes[i].y;
                cout << cluster[j].getNode(cont).y << endl;
                //cluster[j].getNode(cont).n_neighbors = nodes[i].n_neighbors;
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
            if((nodes[i].x < xmin)&&(nodes[i].cluster_index == cluster[j].Index))
                xmin = cluster[j].getNode(i).x;
            if((nodes[i].x > xmax)&&((nodes[i].cluster_index == cluster[j].Index)))
                xmax = cluster[j].getNode(i).x;
            if(cluster[j].getNode(i).y < ymin)
                ymin = cluster[j].getNode(i).y;
            if(cluster[j].getNode(i).y > ymax)
                ymax = cluster[j].getNode(i).y;
        }
    
        if((xmax - xmin)>(ymax - ymin))
            cluster[j].scale = xmax - xmin;
        else
            cluster[j].scale = ymax - ymin;

    }


    for(int j = 0; j < max; j++){
        for(int i = 0; i < cluster[j].N; i++)
            cluster[j].perimeter = cluster[j].perimeter + 4 - cluster[j].getNode(i).n_neighbors;
    }



    for(int j = 0; j <= max; j++)
        cout << "Cluster " << j << " Scale: " << cluster[j].scale << " Perimeter: " << cluster[j].perimeter << "\n";

return 0;

}
