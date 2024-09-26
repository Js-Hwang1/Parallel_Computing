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

    // Start timing
    double start_time = MPI_Wtime();  

    // Make data
    const int N = pow(10, 6);
    vector<float> data(N, 0.0); // Initialize array

    if (mynode == 0) {
        // Central vertex (C1) initializes the data
        for (int i = 0; i < N; i++) {
            data[i] = static_cast<float>(i + 0.1234);  // Example data
        }
    }

    // Make Graph 
    const int num_nodes = 16;

    // Adjacency information for each process (node)
    int index[num_nodes] = {3, 5, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};

    int edges[21] = {
        1, 2, 3,     // 0 to 1, 2, 3
        6, 10,       // 1 to 6, 10
        8, 12,       // 2 to 8, 12
        4, 14,       // 3 to 4, 14
        5,           // 4 to 5
        7,           // 6 to 7
        9,           // 8 to 9
        11,          // 10 to 11
        13,          // 12 to 13
        15           // 14 to 15
    };

    MPI_Comm graph_comm;
    MPI_Graph_create(MPI_COMM_WORLD, num_nodes, index, edges, 1, &graph_comm);

    int num_neighbors;
    MPI_Graph_neighbors_count(graph_comm, mynode, &num_neighbors);

    vector<int> neighbors(num_neighbors);
    MPI_Graph_neighbors(graph_comm, mynode, num_neighbors, neighbors.data());

    // Start the broadcast using Bcast
    MPI_Bcast(data.data(), N, MPI_FLOAT, 0, graph_comm);

    // Stop time and print the time used
    double end_time = MPI_Wtime();
    if (mynode == 0) { // Print only once
        cout << "Broadcast completed in " << end_time - start_time << " seconds.\n";
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
} 
