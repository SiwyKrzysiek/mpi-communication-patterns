#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void mpiClenup()
{
    MPI_Finalize();
}

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    atexit(mpiClenup);

    // Get the number of all processes and rank of current process
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Create pipeline communicator
    MPI_Comm cartesianCommunicator;
    const int dimentionSizes[] = {world_size};
    const int periods[] = {false};
    MPI_Cart_create(MPI_COMM_WORLD, 1, dimentionSizes, periods, true, &cartesianCommunicator);

    int cartesianRank;
    MPI_Comm_rank(cartesianCommunicator, &cartesianRank);
    // printf("From rank %d to %d\n", world_rank, cartesianRank);

    int previousRank, nextRank;
    MPI_Cart_shift(cartesianCommunicator, 0, 1, &previousRank, &nextRank);
    printf("%d: previous is %d, next is %d\n", cartesianRank, previousRank, nextRank);

    return EXIT_SUCCESS;
}