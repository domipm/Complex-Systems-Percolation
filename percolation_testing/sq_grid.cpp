#include<iostream>
#include<fstream>
#include<cmath>

#include "gsl_rng.h"
#include "trees.h"

using namespace std;

ofstream file;

//Number of trees to generate
int N = 100;
//Seed for random numbers
int seed = 932284531; 
//General probability
double P = 0.5;

//Pointer
gsl_rng *tau;

int main() {

    file.open("sq_grid_positions.txt");

    extern gsl_rng *tau;
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,seed);

    Tree tree[N];

    for (int i = 0; i < N; i++) {
       for(int j = 0; j < sqrt(N); j++){            //Comprueba en que row está el arbol i
         if((((j-1)*sqrt(N)) < i) && (i <= (j*sqrt(N))))
          tree[i].y = j;
        } 

      if(abs(tree[i].y) > (2*sqrt(N)))
      tree[i].y = 0.0;
        
        for(int k = 0; k < sqrt(N); k++){       //Comprueba en que column esta el tree i
          if((i % static_cast<int>(sqrt(N))) == k)
            tree[i].x = k;    
        }

  tree[i].p_spawn = gsl_rng_uniform(tau);


        cout << "Tree number " << i+1 << " Pos X = " << tree[i].x << " Pos Y = " << tree[i].y << endl;
      
      if(tree[i].p_spawn < P)  
        file << tree[i].x << "\t" << tree[i].y << "\n";

    }

    file.close();

    return 0;

}