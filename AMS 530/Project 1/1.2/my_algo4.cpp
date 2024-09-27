#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {
    // Setup MPI
    MPI_Init(&argc, &argv);

    int totnodes;
    MPI_Comm_size(MPI_COMM_WORLD, &totnodes);

    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);

    int a=200;  //Reported a broadcast time of 0.004 secs consistant !!!!!!


    const int N = pow(10, 6);  // Size of the data to broadcast
    const int chunk_size = N / a;  // Split the data into 1000 chunks
    vector<float> data(N, 0.0); // Initialize array

    vector<vector<int>> neighbors(16);
    
    neighbors[0] = {1, 2, 3};
    neighbors[1] = {6, 10};
    neighbors[2] = {8, 12};
    neighbors[3] = {4, 14};
    neighbors[4] = {5};
    neighbors[5] = {};
    neighbors[6] = {7};
    neighbors[7] = {};
    neighbors[8] = {9};
    neighbors[9] = {};
    neighbors[10] = {11};
    neighbors[11] = {};
    neighbors[12] = {13};
    neighbors[13] = {};
    neighbors[14] = {15};
    neighbors[15] = {};


    
    
    // Initialize data for broadcasting
    if (mynode == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = static_cast<float>(i + 0.1234);
        }
    } 

    // MPI_Request array for Isend and Irecv
    vector<MPI_Request> requests;

    double start_time = MPI_Wtime();
    if(mynode ==0 ){
        for (int chunk = 0; chunk < a; ++chunk) {
            for (size_t i = 0; i < neighbors[mynode].size(); ++i) {
                MPI_Request req;
                MPI_Isend(&data[chunk * chunk_size], chunk_size, MPI_FLOAT, neighbors[mynode][i], 0, MPI_COMM_WORLD, &req);
                requests.push_back(req);
            }
        }
         MPI_Waitall(requests.size(), &requests[0], MPI_STATUSES_IGNORE);
        
    }
    else {
        // Receive data in chunks and forward to neighbors
        for (int chunk = 0; chunk < a; ++chunk) {
            MPI_Request req_recv;
            MPI_Irecv(&data[chunk * chunk_size], chunk_size, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &req_recv);
            requests.push_back(req_recv);

            // Once a chunk is received, forward it to the process's neighbors
            for (size_t i = 0; i < neighbors[mynode].size(); ++i) {
                MPI_Request req_send;
                MPI_Isend(&data[chunk * chunk_size], chunk_size, MPI_FLOAT, neighbors[mynode][i], 0, MPI_COMM_WORLD, &req_send);
                requests.push_back(req_send);
            }
        }

        // Wait for all non-blocking receives and sends to complete
        MPI_Waitall(requests.size(), &requests[0], MPI_STATUSES_IGNORE);
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
