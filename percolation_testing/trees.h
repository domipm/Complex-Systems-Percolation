#ifndef TREES_H
#define TREES_H

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
        //Determines if the tree spawns
        bool spawn;
        //Labels the cluster that the tree is in
        int cluster_index;
        //¿Vecinos?
        bool next (Tree tree2){
          if(((abs(x - tree2.x) < 1.1)||(abs(y-tree2.y) < 1.1))&&(tree2.spawn == true))
          return true;
          else
          return false; 
        }
        
        


};
//Te dice si tiene vecinos xd
        bool tieneVecinos(Tree& tree1, Tree* tree){
          int cont = 0;
          
          for(int i =0; i < 100; i++)
            if(tree1.next(tree[i]))
              cont++;

          if(cont == 0)
             return false;
             else
             return true;
        }
        
      //Si tiene vecinos que aun no estan en el cluster
      bool tieneVecinosNuevos(Tree& tree1, Tree* tree){
        int cont =0;

       for(int i =0; i < 100; i++)
            if((tree1.next(tree[i]))&&(tree[i].cluster_index == 0))
              cont++;

          if(cont == 0)
             return false;
             else
             return true;

      }

#endif