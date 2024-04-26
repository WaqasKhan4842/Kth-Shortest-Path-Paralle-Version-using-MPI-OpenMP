# Kth-Shortest-Path-Paralle-Version-using-MPI-OpenMP

This project aims to implement parallelized graph processing algorithms using a combination of MPI 
(Message Passing Interface) and OpenMP (Open Multi-Processing).The codebase contains functionalities
to process graphs and find shortest paths efficiently by leveraging parallel computing techniques.

-> To compile only the MPI part of the code.
1. mpic++ -o main main.cpp GraphProcessor.cpp
2. mpiexec -n <num_of_nodes> ./main


-> To compile both MPI and OpenMP.
1. mpic++ -fopenmp main.cpp GraphProcessor.cpp -o main
2. export OMP_NUM_THREADS=<num_of_threads>
3. mpiexec -n <num_of_nodes>  ./main
