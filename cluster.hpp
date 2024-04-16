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

    
        //Characteristic scale of the cluster
    double scale;
        //Perimeter of the cluster
    double perimeter;    

    //Constructor
    Cluster()   {
        N=0; 
        Index=0; 
        scale=0; 
        perimeter=0;
        }
    


};
