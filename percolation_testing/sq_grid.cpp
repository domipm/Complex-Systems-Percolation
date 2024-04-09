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
double P = 0.4;

//Pointer
gsl_rng *tau;

int main() {

    file.open("sq_grid_positions.txt");

    extern gsl_rng *tau;
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,seed);

    Tree tree[N];
    Tree Cluster[N][N];   //1º entrada: nº de cluster // 2º entrada posición en el array
    int elementos[N];

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

      
      tree[i].p_spawn = gsl_rng_uniform(tau);
      if(tree[i].p_spawn < P)
        tree[i].spawn = true;

cout << "Tree number " << i+1 << " Pos X = " << (int)tree[i].x << " Pos Y = " << (int)tree[i].y << " P = " << tree[i].p_spawn << "\n";
    
    }
    
    
                                                         //Search for clusters
    for(int i = 0; i < N; i++){
     tree[i].cluster_index = 0;
     elementos[i]=0; 
    }


    int cont=1;
    
     for (int i = 0; i < N; i++){
      int t=0;
       if((tree[i].spawn == true)&&(tree[i].cluster_index == 0)){   //Si todavia no estaba en un 
        tree[i].cluster_index = cont;                              //cluster lo convierte en el 
        Cluster[cont][0]=tree[i];                                 //elemento 0 de su cluster   
        elementos[cont]=1;
       

       bool condition = true;

       while (condition == true){                     //condition true if tiene vecinos nuevos
        cout << t << "\n";

        int c = 0;
        for(int j = 0; j < elementos[cont]; j++)           
         if(tieneVecinosNuevos(Cluster[cont][j], tree) == true)
           c++;

         if(c != 0)  
          condition == true;
          if(c == 0)
          condition == false;


        
        if (t>15000)
          condition = false;
        


        for (int k = 0; k < elementos[cont]; k++){             //Recorrer los elementos del cluster y 
           for (int l = 0; l < N; l++)                   //          añadir 1 "capa de vecinos"
              if((Cluster[cont][k].next(tree[l]))&&(tree[l].cluster_index == 0)){
                tree[l].cluster_index = cont;
                elementos[cont]++;
                Cluster[cont][elementos[cont]]=tree[l];
              }        
        }

        t++;
       }

       cont++;
       }
     }


    for(int j = 0; j < N; j++)
      if(tree[j].spawn == true)
      file << (int)tree[j].x << "\t" << (int)tree[j].y << "\t" << tree[j].cluster_index << "\n";


    file.close();

    return 0;

}