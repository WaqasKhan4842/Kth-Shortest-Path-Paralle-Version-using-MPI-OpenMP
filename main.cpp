#include <iostream>
#include <mpi.h>
#include "Menu.h"
#include "GraphProcessor.h"

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    GraphProcessor processor;
    int choice;
    int sub_choice;
    double start_time, end_time;

    if (rank == 0)
    {
        choice = menu(); // Only rank 0 executes the menu
        if (choice == 1)
            sub_choice = choose_from_SNAP();
        else if (choice == 2)
            sub_choice = choose_from_Doctor_Who();
    }

    // Broadcast the choice selected by rank 0 to all processes
    MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast the sub_choice selected by rank 0 to all processes
    MPI_Bcast(&sub_choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
    switch (choice)
    {
    case 1:
        processor.read_file_SNAP(sub_choice);
        if (rank == 0)
        {
            start_time = MPI_Wtime(); // Start time for the whole program
            // Serial execution by rank 0
            processor.serial_execution(1);
        }

        // Barrier synchronization to ensure all processes wait until rank 0 completes serial part
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == 0)
        {
            end_time = MPI_Wtime(); // End time for the whole program
            double serial_time = end_time - start_time;
            cout << "Serial execution time: " << serial_time << " seconds" << endl;
        }

        if (rank == 0)
        {
            start_time = MPI_Wtime(); // Start time for parallel execution
        }
        MPI_Barrier(MPI_COMM_WORLD);
        processor.parallel_execution(rank, size, 1);
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0)
        {
            end_time = MPI_Wtime(); // End time for parallel execution
            double parallel_time = end_time - start_time;
            cout << "Parallel execution time: " << parallel_time << " seconds" << endl;
        }
        break;
    case 2:
        processor.read_file_DoctorWho(sub_choice);

        if (rank == 0)
        {
            start_time = MPI_Wtime(); // Start time for the whole program
            // Serial execution by rank 0
            processor.serial_execution(2);
        }

        // Barrier synchronization to ensure all processes wait until rank 0 completes serial part
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == 0)
        {
            end_time = MPI_Wtime(); // End time for the whole program
            double serial_time = end_time - start_time;
            cout << "Serial execution time: " << serial_time << " seconds" << endl;
        }

        if (rank == 0)
        {
            start_time = MPI_Wtime(); // Start time for parallel execution
        }
        MPI_Barrier(MPI_COMM_WORLD);
        processor.parallel_execution(rank, size, 2);
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0)
        {
            end_time = MPI_Wtime(); // End time for parallel execution
            double parallel_time = end_time - start_time;
            cout << "Parallel execution time: " << parallel_time << " seconds" << endl;
        }

        break;
    }
}