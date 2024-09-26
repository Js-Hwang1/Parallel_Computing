#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

int main(int argc, char **argv) {
    // Setup MPI
    MPI_Init(&argc, &argv);

    int totnodes;
    MPI_Comm_size(MPI_COMM_WORLD, &totnodes);

    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);

    // Define constants
    const int N = pow(10, 6);  // Size of the data to broadcast
    vector<float> data(N, 0.0); // Initialize array

    double start_time = MPI_Wtime();
    MPI_Request requests[3];  // Array to hold send/receive requests


    if (mynode == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = static_cast<float>(i + 0.1234);  
        }
        for (int i = 1; i < 4; i++) {
            MPI_Isend(&data[0], N, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &requests[i - 1]);
        }
  
        MPI_Waitall(3, requests, MPI_STATUSES_IGNORE);
    }

    if (mynode == 1) {
        MPI_Irecv(&data[0], N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 6, 0, MPI_COMM_WORLD, &requests[1]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 10, 0, MPI_COMM_WORLD, &requests[2]);

        MPI_Waitall(3, requests, MPI_STATUSES_IGNORE);
    }

    if (mynode == 6) {
        MPI_Irecv(&data[0], N, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 7, 0, MPI_COMM_WORLD, &requests[1]);

        MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
    }

    if (mynode == 10) {
        MPI_Irecv(&data[0], N, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 11, 0, MPI_COMM_WORLD, &requests[1]);

        MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
    }

    if (mynode == 2) {
        MPI_Irecv(&data[0], N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 8, 0, MPI_COMM_WORLD, &requests[1]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 12, 0, MPI_COMM_WORLD, &requests[2]);

        MPI_Waitall(3, requests, MPI_STATUSES_IGNORE);
    }

    if (mynode == 8) {
        MPI_Irecv(&data[0], N, MPI_FLOAT, 2, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 9, 0, MPI_COMM_WORLD, &requests[1]);

        MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
    }

    if (mynode == 12) {
        MPI_Irecv(&data[0], N, MPI_FLOAT, 2, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 13, 0, MPI_COMM_WORLD, &requests[1]);

        MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
    }

    if (mynode == 3) {
        MPI_Irecv(&data[0], N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 4, 0, MPI_COMM_WORLD, &requests[1]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 14, 0, MPI_COMM_WORLD, &requests[2]);

        MPI_Waitall(3, requests, MPI_STATUSES_IGNORE);
    }

    if (mynode == 4) {
        MPI_Irecv(&data[0], N, MPI_FLOAT, 3, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 5, 0, MPI_COMM_WORLD, &requests[1]);


        MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
    }

    if (mynode == 14) {
        MPI_Irecv(&data[0], N, MPI_FLOAT, 3, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&data[0], N, MPI_FLOAT, 15, 0, MPI_COMM_WORLD, &requests[1]);

        MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
    }

    // Stop timing
    double end_time = MPI_Wtime();

    // Central node prints timing results
    if (mynode == 0) {
        cout << "Custom broadcast completed in " << end_time - start_time << " seconds." << endl;
    }

    MPI_Finalize();

    return 0;
}
