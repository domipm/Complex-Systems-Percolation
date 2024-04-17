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

    //Constructor
    Cluster()   {
        tree = new Node[N_MAX];
        for (int i = 0; i < N_MAX; i++)
            tree[i].index = i;
        N=0; 
        Index=0; 
        scale=0; 
        perimeter=0;
    } 

        //Access the element 'pos' of the cluster
    Node& getNode(int pos){
        return tree[pos];
    }
        //Characteristic scale of the cluster
    int scale;
        //Perimeter of the cluster
    int perimeter;    
    
};
