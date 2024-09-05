#include <mpi.h> // Import MPI header file 
#include <iostream> // Import IO header file for printing

using namespace std;

int main(int argc, char** argv) {

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Print a hello world message from each processor
    cout << "Hello from Processor " << world_rank << endl;

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
