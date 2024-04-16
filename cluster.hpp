#pragma once
#include<math.h>
#include<stdio.h>
#include "node.hpp"

class Cluster{

    public:

            //Very large number
    int GreatN = 10000;        

        //Number of elements
    int N;
        //Index
    int Index;

                                //elements of the cluster
    Node* tree;    

    cluster(int _GreatN) : N(_GreatN){
        tree = new Node[GreatN];     //Assign memory for the array
        for (int i = 0; i < N; ++i) {
        tree[i] = Node(); // Call default constructor of Node for each element
    }
    }

    ~cluster(){
        delete[] tree;          //Frees the assigned memory
    }

        //Access the element 'pos' of the cluster
    Node& getNode(int pos){
        return tree[pos];
    }
        //Characteristic scale of the cluster
    double scale;
        //Perimeter of the cluster
    double perimeter;    

    //Constructor
    cluster()   {
        N=0; 
        Index=0; 
        scale=0; 
        perimeter=0;
        }
    


};
