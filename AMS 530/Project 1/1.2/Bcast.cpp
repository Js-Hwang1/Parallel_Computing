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
    
    // Make data
    const int N = pow(10, 6);
    vector<float> data(N, 0.0); // Initialize array

    if (mynode == 0) {
        // Central vertex (C1) initializes the data
        for (int i = 0; i < N; i++) {
            data[i] = roundf((static_cast<float>(i) + 0.1234) * 10000) / 10000;  // Round to nearest tenth
        }
    }

    const int num_nodes = 16;

    int index[num_nodes] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48};

    int edges[48] = {
        1, 2, 3,
        0, 6, 10,
        0, 8, 12,
        0, 4, 14,
        3, 5, 9,
        4, 6, 11,
        1, 5, 7,
        6, 8, 13,
        2, 7, 9,
        4, 8, 15,
        1, 11, 15,
        5, 10, 12,
        2, 11, 13,
        7, 12, 14,
        3, 13, 15,
        9, 10, 14
    };

    MPI_Comm graph_comm;
    MPI_Graph_create(MPI_COMM_WORLD, num_nodes, index, edges, 1, &graph_comm);

    int num_neighbors;
    MPI_Graph_neighbors_count(graph_comm, mynode, &num_neighbors);

    vector<int> neighbors(num_neighbors);
    MPI_Graph_neighbors(graph_comm, mynode, num_neighbors, neighbors.data());
    
    double start_time = MPI_Wtime();  

    MPI_Bcast(data.data(), N, MPI_FLOAT, 0, graph_comm);

    double end_time = MPI_Wtime();


    MPI_Barrier(graph_comm); 

    bool is_successful = true; 
    for (int j = 0; j < N; j++) {
        float expected_value = roundf((static_cast<float>(j) + 0.1234) * 10000) / 10000; 
        if (fabs(data[j] - expected_value) > 0.00001) { 
            cout << "Broadcast incomplete in node " << mynode << endl;
            is_successful = false;  
            break;  
        }
    }

    int success_flag = is_successful ? 1 : 0;
    if (mynode != 0) {
        MPI_Send(&success_flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        vector<int> success_flags(totnodes, 1);  
        for (int i = 1; i < totnodes; i++) {
            MPI_Recv(&success_flags[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        bool all_successful = true;
        for (int i = 0; i < totnodes; i++) {
            if (success_flags[i] != 1) {
                all_successful = false;
                break;
            }
        }
        if (all_successful) {
            cout <<end_time - start_time<< endl;
        }
    }
    MPI_Finalize();

    return 0;
}
