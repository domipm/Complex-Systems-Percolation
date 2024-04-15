#pragma once
#include<math.h>
#include<stdio.h>
#include "node.hpp"

class cluster{

    public:

        //Number of elements
    int N;
        //Index
    int Index;

                                //elements of the cluster
    Node* tree;    

    cluster(int _N) : N(_N){
        tree = new Node[N];     //Assign memory for the array
    }

    ~cluster(){
        delete[] tree;          //Frees the assigned memory
    }

        //Access the element 'index' of the cluster
    Node& getNode(int index){
        return tree[index];
    }
        //Characteristic scale of the cluster
    double scale;
        //Perimeter of the cluster
    double perimeter;    

    //Constructor
    cluster() : tree(nullptr), N(0), Index(0), scale(0), perimeter(0) {}
    cluster(int _N) : N(_N), Index(0), scale(0), perimeter(0) {
        tree = new Node[N]; // Asigna memoria para el arreglo de nodos
        // Inicializa cada nodo del árbol
        for (int i = 0; i < N; ++i) {
            tree[i] = Node(); // Llama al constructor predeterminado de Node para cada elemento
    }
    }    


};
