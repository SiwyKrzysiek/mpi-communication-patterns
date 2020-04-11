#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define VECTOR_LENGHT 1000000
#define PART_LENGTH 1000

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
    // printf("%d: previous is %d, next is %d\n", cartesianRank, previousRank, nextRank);

    int *vector;
    if (cartesianRank == 0) // First process has vector
    {
        vector = alloca(VECTOR_LENGHT * sizeof(*vector));
        memset(vector, 0, VECTOR_LENGHT * sizeof(*vector));
    }

    long long sum = 0; // For last process

    for (int i = 0; i < VECTOR_LENGHT / PART_LENGTH; i++)
    {
        int part[PART_LENGTH];

        if (cartesianRank == 0) // First process starts sending data
        {
            int shift = i * PART_LENGTH;
            memcpy(part, vector + shift, PART_LENGTH * sizeof(*part));
        }

        MPI_Recv(part, PART_LENGTH, MPI_INT, previousRank, 0, cartesianCommunicator, MPI_STATUS_IGNORE);

        if (cartesianRank != 0 && cartesianRank != world_size-1) // Process in the middle transforms data
        {
            for (int i = 0; i < PART_LENGTH; i++)
            {
                part[i] = part[i] + 1;
            }
        }
        else if(cartesianRank == world_size-1) // Last process calculates sum of received values
        {
            for (int i = 0; i < PART_LENGTH; i++)
            {
                sum += part[i];
            }
        }

        MPI_Send(part, PART_LENGTH, MPI_INT, nextRank, 0, cartesianCommunicator);
    }

    if (cartesianRank == world_size-1) // Last process
    {
        printf("Sum of received values: %lld\n", sum);
    }

    return EXIT_SUCCESS;
}