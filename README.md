# Complex-Systems-Percolation

Code used for the project "Fractals and Percolation" in the course Physics of Complex Systems.

The file "percolation.hpp" contains the necessary Node, Cluster, and Lattice classes used in the codes to generate the results. "lattice_generator.cpp" generates the lattices with the desired initial parameters, "hoshen-kopelman.cpp" uses algorithm that sorts the nodes into clusters given a neighboring threshold distance, and finally "cluster_stats.cpp" computes the necessary parameters (such as size and perimeter) of each cluster.

The python script "graph_sorting.py" plots the lattices for the initial and sorted (after applying Hoshen-Koppelman algorithm) lattices, and serves to visualize better the results and see the different clusters formed.

In the different folders are located additional percolation use cases and examples. Inside "./random 2d grid" and "./random 3d grid" are self-contained codes that generate two- and three-dimensional lattices and find clusters via the previously mentioned algorithm. "./galaxy percolation" contains a python script that studies the star-forming regions in spiral galaxies from the point of view of percolation systems. Finally, "./forest fire" contains all the necessary code to solve the problem proposed in the project, and that is how many trees would we have to cut in order to stop the spread of fire in a forest.

Apart from the standard C++ and Python libraries, we also make use of the GSL library (in particular, the module "gsl_rng" used to generate random numbers).

<a href="url"><img src="https://gcdnb.pbrd.co/images/Oktt63RPLirx.gif?o=1" height="512" width="512" class="center" ></a>

