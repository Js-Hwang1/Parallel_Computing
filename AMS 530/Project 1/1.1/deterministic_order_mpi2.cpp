#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int totnodes;
    MPI_Comm_size(MPI_COMM_WORLD, &totnodes);

    // Get the rank of the process
    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);

    
    if(mynode == 0){
        cout<<"Hello world from processor " << mynode<<endl;
        MPI_Send(&mynode,1,MPI_INT,mynode+1,0,MPI_COMM_WORLD);
    }else{
        int number = 0;
        MPI_Recv(&number, 1, MPI_INT,mynode-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        cout<<"Hello world from processor " <<  mynode<<endl;
        if(mynode < totnodes-1){
            MPI_Send(&mynode,1,MPI_INT,mynode+1,0,MPI_COMM_WORLD);
        }
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
