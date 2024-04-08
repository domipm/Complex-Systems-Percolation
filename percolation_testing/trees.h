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
        bool next (tree tree2){
          if((abs(x - tree2.x) < 1.1)||(abs(y-tree2.y) < 1.1))
          return true;
          else
          return false; 
        }

};

#endif