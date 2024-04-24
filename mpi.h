#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <mpi.h>

using namespace std;

void findKShortest(int **edges, int n, int m, int k, int source, int dest, int rank, int size)
{
    // Initialize MPI
    MPI_Init(NULL, NULL);

    // Get the rank and size
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Calculate the number of vertices per process
    int vertices_per_process = n / size;

    // Calculate the start and end vertices for this process
    int start_vertex = rank * vertices_per_process;
    int end_vertex = start_vertex + vertices_per_process;

    // Initialize graph
    vector<vector<pair<int, int>>> g(n + 1);
    for (int i = 0; i < m; i++)
    {
        // Storing edges
        g[edges[i][0]].push_back(make_pair(edges[i][1], edges[i][2]));
    }

    // Vector to store distances
    vector<vector<int>> dis(n + 1, vector<int>(k, 1e9));

    // Initialization of priority queue
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, source));
    dis[source][0] = 0;

    // while pq has elements
    while (!pq.empty())
    {
        // Storing the node value
        int u = pq.top().second;

        // Storing the distance value
        int d = pq.top().first;
        pq.pop();
        if (dis[u][k - 1] < d)
            continue;
        vector<pair<int, int>> v = g[u];

        // Traversing the adjacency list
        for (int i = 0; i < v.size(); i++)
        {
            int destNode = v[i].first;
            int cost = v[i].second;

            // Checking for the cost
            if (d + cost < dis[destNode][k - 1])
            {
                dis[destNode][k - 1] = d + cost;

                // Sorting the distances
                sort(dis[destNode].begin(), dis[destNode].end());

                // Pushing elements to priority queue
                pq.push(make_pair(d + cost, destNode));
            }
        }
    }

    // Printing K shortest paths from source to destination
    cout << "K shortest paths from node " << source << " to node " << dest << " for process " << rank << ":" << endl;
    for (int i = 0; i < k; i++)
    {
        cout << "Path " << i + 1 << ": " << dis[dest][i] << endl;
    }

    // Finalize MPI
    MPI_Finalize();
}

int main(int argc, char *argv[])
{
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the rank and size
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Sample usage
    int n = 4;   // Number of nodes
    int m = 6;   // Number of edges
    int k = 3;   // Number of shortest paths to find
    int source = 1; // Source node
    int dest = 4;   // Destination node

    // Sample edges
    int **edges = new int *[m];
    for (int i = 0; i < m; ++i)
    {
        edges[i] = new int[3];
    }

    edges[0][0] = 1; edges[0][1] = 2; edges[0][2] = 1;
    edges[1][0] = 1; edges[1][1] = 3; edges[1][2] = 3;
    edges[2][0] = 2; edges[2][1] = 3; edges[2][2] = 2;
    edges[3][0] = 2; edges[3][1] = 4; edges[3][2] = 6;
    edges[4][0] = 3; edges[4][1] = 2; edges[4][2] = 8;
    edges[5][0] = 3; edges[5][1] = 4; edges[5][2] = 1;

    // Find k shortest paths from source to destination
    findKShortest(edges, n, m, k, source, dest, rank, size);

    // Deallocate memory
    for (int i = 0; i < m; ++i)
    {
        delete[] edges[i];
    }
    delete[] edges;

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
