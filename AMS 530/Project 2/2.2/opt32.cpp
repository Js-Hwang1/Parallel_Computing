#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int totnodes;
    MPI_Comm_size(MPI_COMM_WORLD, &totnodes);

    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    
    const int N = pow(10,1);
    vector<float> data(N, 0.0); 


    for (int i = 0; i < N; i++) {
        data[i] = (roundf(mynode + static_cast<float>(i)*.01f) * 10000) / 10000; 
    }

    vector<float> recv(totnodes*32); 

    for(int k = 0; k <N;k++){
        recv[k] = data[k]; 
    }
    
    const int num_nodes = 32;

    int index[num_nodes] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96};

    int edges[96] = {
        //0
        1,16,31,
        //1
        0,2,23,
        //2
        1,3,28,
        //3
        2,5,19,
        //4
        3,5,10,
        //5
        4,6,14,
        //6
        5,7,29,
        //7
        6,8,17,
        //8
        7,9,24,
        //9
        8,10,31,
        //10
        9,11,4,
        //11
        10,12,27,
        //12
        11,13,18,
        //13
        12,14,22,
        //14
        5,13,15,
        //15
        14,16,25,
        //16
        0,15,17,
        //17
        7,16,18,
        //18
        12,17,19,
        //19
        3,18,20,
        //20
        19,21,26,
        //21
        20,22,30,
        //22
        13,21,23,
        //23
        1,22,24,
        //24
        8,23,25,
        //25
        15,24,26,
        //26
        20,25,27,
        //27
        11,26,28,
        //28
        2,27,29,
        //29
        6,28,30,
        //30
        21,29,31,
        //31
        0,9,30

    };

    MPI_Comm graph_comm;
    MPI_Graph_create(MPI_COMM_WORLD, num_nodes, index, edges, 1, &graph_comm);

    int num_neighbors;
    MPI_Graph_neighbors_count(graph_comm, mynode, &num_neighbors);

    vector<int> neighbors(num_neighbors);
    MPI_Graph_neighbors(graph_comm, mynode, num_neighbors, neighbors.data());
    
    double start_time = MPI_Wtime();  

    MPI_Allgather(data.data(), N, MPI_FLOAT, recv.data(), N, MPI_FLOAT, graph_comm);

    double end_time = MPI_Wtime();

    double time_taken = roundf((end_time - start_time) * 1000000) / 1000000;

    float veri = 0.0;
    for (int i = 0; i < totnodes; i++) {
        for (int j = 0; j < N; j++) {
            veri += (roundf(i + static_cast<float>(j)*.01f) * 10000) / 10000;
        }
    }

    float sum = 0.0;
    for (int i = 0; i < totnodes; i++) {
        for (int j = 0; j < N; j++) {
            sum += recv[i * N + j]; 
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (mynode != 0) {
        MPI_Request request;
        MPI_Isend(&time_taken, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }

    if (mynode == 0) {

        double average_time = time_taken;

        vector<double> received_times(31);
        vector<MPI_Request> recv_requests31(31);
        
        for (int i = 0; i < 31; ++i) {
            MPI_Irecv(&received_times[i], 1, MPI_DOUBLE, 1+i, 0, MPI_COMM_WORLD, &recv_requests31[i]);
        }
        MPI_Waitall(31, recv_requests31.data(), MPI_STATUSES_IGNORE);

        for(int i = 0;i<31;++i){
          average_time += received_times[i];
        }
        average_time = average_time/32;

        double standard_dev = pow(time_taken-average_time,2);

        for(int i = 0;i<31;++i){
          standard_dev += pow(average_time-received_times[i],2);
        }
        standard_dev = pow(standard_dev/32,.5);


        cout<< average_time <<"  "<<standard_dev <<'\n';
    }

    MPI_Finalize();

    return 0;
}

/* USED FOR DEBUGGING
// Verification check
    if (sum == veri) {
        cout << "ALLGATHER COMPLETE AT NODE " << mynode << " Time: " <<end_time -start_time << '\n';
    }else{
        cout << "ALLGATHER INCOMPLETE AT NODE " << mynode << '\n';
    }
*/
