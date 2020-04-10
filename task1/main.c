#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int nwd(int a, int b);

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of all processes and rank of current process
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    printf("%d\n", nwd(15, 57));

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