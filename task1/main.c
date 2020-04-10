#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

// Calculate greates common dividor of a and b
int nwd(int a, int b);

// Simple implementation of accurate exponentiation
int simplePow(int number, int exponent);

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of all processes and rank of current process
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // TODO: Read numbers from args
    // For now use fixed values
    const int values[] = {15, 21, 24, 57};
    assert(world_size == 4);

    const int totalSteps = log2(world_size);
    if (world_rank == 0)
        printf("Calculations will take %d steps\n\n", totalSteps);

    int myValue = values[world_rank];
    for (int step = 0; step < totalSteps; step++)
    {
        const int shift = simplePow(2, step);
        const int sendToRank = (world_rank + shift) % world_size;
        int reciveFromRank = world_rank - shift;
        if (reciveFromRank < 0)
            reciveFromRank += world_size;

        printf("Itearton %d -> Process %d sends to %d and recieves from %d\n", step, world_rank, sendToRank, reciveFromRank);

        int partnerValue;
        MPI_Sendrecv(&myValue, 1, MPI_INT, sendToRank, 0,
                     &partnerValue, 1, MPI_INT, reciveFromRank, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        myValue = nwd(myValue, partnerValue);
    }

    printf("Program %d -> Result: %d\n", world_rank, myValue);

    MPI_Finalize();
    return EXIT_SUCCESS;
}

// Calculate greates common dividor of a and b
int nwd(int a, int b)
{
    if (b > a) // Swap if necessary
    {
        int tmp = a;
        a = b;
        b = tmp;
    }

    while (b != 0)
    {
        int c = b;
        b = a % b;
        a = c;
    }

    return a;
}

int simplePow(int number, int exponent)
{
    int result = 1;
    while (exponent--)
        result *= number;

    return result;
}