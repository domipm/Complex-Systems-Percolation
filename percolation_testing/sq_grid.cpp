#include<iostream>
#include<fstream>
#include<cmath>

#include "gsl_rng.h"
#include "trees.h"

using namespace std;

ofstream file;

//Number of trees to generate
int N = 100;
//Side length of the forest
int L = 10;
//Seed for random numbers
int seed = 932284531; 
//General probability
double P = 0.7;

//Pointer
gsl_rng *tau;

int main() {

    file.open("sq_grid_positions.txt");

    extern gsl_rng *tau;
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,seed);

    Tree tree[N];
    Tree Cluster[N][N];   //1º entrada: nº de cluster // 2º entrada posición en el array


                                      //Generates Grid
    for (int i = 0; i < N; i++) {
       for(int j = 0; j < sqrt(N); j++){            //Comprueba en que row está el arbol i
         if((((j-1)*sqrt(N)) < i) && (i <= (j*sqrt(N))))
          tree[i].y = j;
        } 

      if(abs(tree[i].y) > (2*sqrt(N)))
      tree[i].y = 0;
        
        for(int k = 0; k < sqrt(N); k++){       //Comprueba en que column esta el tree i
          if((i % (int)(sqrt(N))) == k)
            tree[i].x = k;    
        }

      cout << "Tree number " << i+1 << " Pos X = " << (int)tree[i].x << " Pos Y = " << (int)tree[i].y << endl;
  
  
      tree[i].p_spawn = gsl_rng_uniform(tau);
      if(tree[i].p_spawn < P)
        tree[i].spawn = true;

          
      if(tree[i].spawn == true)  
        file << (int)tree[i].x << "\t" << (int)tree[i].y << "\n";
    }
    
                                                         //Search for clusters
    for(int i = 0; i < N; i++)
     tree[i].cluster_index = 0;

    int cont=1;

     for (int i = 0; i < N; i++){
       if((tree[i].spawn == true)&&(tree[i].cluster_index == 0)){
        tree[i].cluster_index = cont;
        Cluster[cont][0]=tree[i];
       }

       while (bool condition == true){
        int elementos = 1;
        for(int j = 0; j < N; j++)
         if(Cluster[cont][j].tieneVecinosNuevos(tree[N]))
          condition == true;
          else
          condition == false;


        for (int k = 1; k < N; k++){               //Recorrer los elementos del cluster y 
           for (int l = 0; l < N; l++)             //             añadir 1 "capa de vecinos"
              if(Cluster[cont][k].next(tree[l])){
                tree[l].cluster_index = cont;
                elementos++;
                Cluster[cont][elementos]=tree[l];
              }
                
        
        
        
        }
        
       }

       cont++;
     }


    file.close();

    return 0;

}