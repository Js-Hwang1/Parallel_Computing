#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {
    // Intialize
    MPI_Init(&argc, &argv);

    int totnodes;
    MPI_Comm_size(MPI_COMM_WORLD, &totnodes);

    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);

    int a = 1; 
    const int N = pow(10, 6);  
    const int chunk_size = N / a;  
    vector<float> data(N, 0.0); 

    // Initialize data for broadcasting at mynode == 0 (C1)
    if (mynode == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = roundf((static_cast<float>(i) + 0.1234) * 10000) / 10000;  // Round to avoid any rounding errors
        }
    }

    // Neighbors list for each node (based on the reduced tree topology of the given graph)
    vector<vector<int>> neighbors(16);
    neighbors[0] = {1, 2, 3};
    neighbors[1] = {6, 10};
    neighbors[2] = {8, 12};
    neighbors[3] = {4, 14};
    neighbors[4] = {5};
    neighbors[6] = {7};
    neighbors[8] = {9};
    neighbors[10] = {11};
    neighbors[12] = {13};
    neighbors[14] = {15};

    

    vector<MPI_Request> recv_requests(a), send_requests(a * neighbors[mynode].size());


    // Start Sending
    double start_time = MPI_Wtime();
    if (mynode == 0) {
        for (int chunk = 0; chunk < a; ++chunk) {
            for (size_t i = 0; i < neighbors[mynode].size(); ++i) {
                int dest = neighbors[mynode][i];
                MPI_Isend(&data[chunk * chunk_size], chunk_size, MPI_FLOAT, dest, chunk, MPI_COMM_WORLD, &send_requests[chunk * neighbors[mynode].size() + i]);
            }
        }
        MPI_Waitall(a * neighbors[mynode].size(), send_requests.data(), MPI_STATUSES_IGNORE);
    }

    if (mynode != 0) {

        for (int chunk = 0; chunk < a; ++chunk) {
            MPI_Irecv(&data[chunk * chunk_size], chunk_size, MPI_FLOAT, MPI_ANY_SOURCE, chunk, MPI_COMM_WORLD, &recv_requests[chunk]);
            MPI_Wait(&recv_requests[chunk], MPI_STATUSES_IGNORE);

            for (size_t i = 0; i < neighbors[mynode].size(); ++i) {
                int dest = neighbors[mynode][i];
                MPI_Isend(&data[chunk * chunk_size], chunk_size, MPI_FLOAT, dest, chunk, MPI_COMM_WORLD, &send_requests[chunk * neighbors[mynode].size() + i]);
            }
        }
        MPI_Waitall(a * neighbors[mynode].size(), send_requests.data(), MPI_STATUSES_IGNORE);
    }
    // Finish timeing and calculate broadcast time
    double end_time = MPI_Wtime();

    double time_taken = roundf((end_time - start_time) * 1000000) / 1000000;
    
    MPI_Barrier(MPI_COMM_WORLD);
    if (mynode == 5 || mynode == 7 || mynode == 9 || mynode == 11 || mynode == 13) {
        
        MPI_Request request;
        MPI_Isend(&time_taken, 1, MPI_DOUBLE, 15, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }

    double final_time=0.0;
    vector<int> leaves(5);
    leaves[0] = 5;
    leaves[1] = 7;
    leaves[2] = 9;
    leaves[3] = 11;
    leaves[4] = 13;

    if (mynode == 15) {

        final_time = time_taken;

        vector<double> received_times(5);
        vector<MPI_Request> recv_requests15(5);
        
        for (int i = 0; i < 5; ++i) {
            MPI_Irecv(&received_times[i], 1, MPI_DOUBLE, leaves[i], 0, MPI_COMM_WORLD, &recv_requests15[i]);
        }
        MPI_Waitall(5, recv_requests15.data(), MPI_STATUSES_IGNORE);

        for(int i = 0;i<5;++i){
            if (received_times[i] > final_time){
                final_time = received_times[i];
            }
        }

    }

    // Verify broadcast by checking all elements one by one
    int is_successful = 1; 
    for (int j = 0; j < N; j++) {
        float expected_value = roundf((static_cast<float>(j) + 0.1234) * 10000) / 10000; 
        if (fabs(data[j] - expected_value) > 0.000000001) { 
            cout << "Broadcast incomplete in node " << mynode << endl;
            is_successful = 0;  
            break;  
        }
    }

    if(mynode != 15) {
        MPI_Send(&is_successful, 1, MPI_INT, 15, 0, MPI_COMM_WORLD);
    }
    // If verification was a success print broadcast time
    if (mynode == 15) {
        vector<int> is_successfuls(totnodes, 1); 
        for (int i = 0; i < totnodes - 1; i++) { 
            MPI_Recv(&is_successfuls[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        is_successfuls[15] = is_successful; 

        bool all_successful = true;
        for (int i = 0; i < totnodes; i++) {
            if (is_successfuls[i] != 1) {
                all_successful = false;
                break;
            }
        }
        if (all_successful) {
            cout << final_time << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
