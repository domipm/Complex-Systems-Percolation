#include<iostream>
#include<cmath>
#include"gsl_rng.h"

using namespace std;

//Tree class
class Tree {

    public:
        //Position of the tree
        float x,y; 
        //Size of the tree?
        float size;
        //Probabilities of the particular tree spawning in, burning out, and spreading the fire
        float p_spawn, p_burn, p_spread;
        //Connections of the tree to others (in a grid?) (N,S,W,E cardinal directions)
        char dir;

};

int main() {

    return 0;

}