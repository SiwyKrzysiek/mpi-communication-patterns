#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Calculate greates common dividor of a and b
int nwd(int a, int b);

// Simple implementation of accurate exponentiation
int simplePow(int number, int exponent);

// Program arguments count should match number of prcesses and all should be numbers
bool validateAndParseArguments(int argc, char **argv, int* valuesOut);

void mpiClenup()
{
    MPI_Finalize();
}

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    atexit(mpiClenup);

    // Get the number of all processes and rank of current process
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int values[world_size];
    if (!validateAndParseArguments(argc, argv, values))
        return EXIT_FAILURE;

    const int totalSteps = log2(world_size);
#ifdef DEBUG
    if (world_rank == 0)
        printf("Calculations will take %d steps\n\n", totalSteps);
#endif

    int myValue = values[world_rank];
    for (int step = 0; step < totalSteps; step++)
    {
        const int shift = simplePow(2, step);
        const int sendToRank = (world_rank + shift) % world_size;
        int reciveFromRank = world_rank - shift;
        if (reciveFromRank < 0)
            reciveFromRank += world_size;
#ifdef DEBUG
        printf("Itearton %d -> Process %d sends to %d and recieves from %d\n", step, world_rank, sendToRank, reciveFromRank);
#endif

        int partnerValue;
        MPI_Sendrecv(&myValue, 1, MPI_INT, sendToRank, 0,
                     &partnerValue, 1, MPI_INT, reciveFromRank, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        myValue = nwd(myValue, partnerValue);
    }

#ifdef DEBUG
    printf("Program %d -> Result: %d\n", world_rank, myValue);
#else
    if (world_rank == 0)
        printf("GCD of all provided nubmers is %d\n", myValue);
#endif

    return EXIT_SUCCESS;
}

bool validateAndParseArguments(int argc, char **argv, int* valuesOut)
{
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (argc-1 != world_size)
    {
        if (world_rank == 0)
            printf("Argumetns number doesn't match processes number\n"
                   "Expecetd %d, recived %d\n", world_size, argc-1);
        return false;
    }

    for (int i = 0; i < argc-1; i++)
    {
        int value = atoi(argv[i+1]);
        if (value <= 0)
        {
            if (world_rank == 0)
                printf("Argument number %d is not valid. Must be a positive nuber\n", i+1);
            return false;
        }

        valuesOut[i] = value;
    }

    return true;
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