#include<iostream>
#include<fstream>
#include<cmath>

#include"gsl_rng.h"

#include "trees.h"

using namespace std;

ofstream file;

//Number of trees to generate
int N = 100;
//Seed for random numbers
int seed = 932284531; 

//Pointer
gsl_rng *tau;

int main() {

    file.open("grid_positions.txt");

    extern gsl_rng *tau;
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,seed);

    Tree tree[N];

    for (int i = 0; i < N; i++) {

        tree[i].x = gsl_rng_uniform(tau);
        tree[i].y = gsl_rng_uniform(tau);

        cout << "Tree number " << i+1 << " Pos X = " << tree[i].x << " Pos Y = " << tree[i].y << endl;
        
        file << tree[i].x << "," << tree[i].y << ","; //<< "\t";

    }

    file.close();

    return 0;

}