#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <omp.h>
using namespace std;

// Function to find K shortest path lengths
void findKShortest(const vector<vector<pair<int, int>>> &g, int n, int m, int k, int source, int dest, int dataset_type, int execution_type)
{
    // Vector to store distances
    vector<vector<int>> dis(n + 1, vector<int>(k, 1e9));

    // Initialization of priority queue
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, source));
    dis[source][0] = 0;
    int numPaths = min(k, m); // Declare and initialize numPaths before the parallel region

    // while pq has elements
    while (!pq.empty())
    {
        // Storing the node value
        int u = pq.top().second;

        // Storing the distance value
        int d = (pq.top().first);
        pq.pop();
        if (dis[u][k - 1] < d)
            continue;
        vector<pair<int, int>> v = g[u];

        if (execution_type == 1)
        {
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
                    std::sort(dis[destNode].begin(), dis[destNode].end());

                    // Pushing elements to priority queue
                    pq.push(make_pair(d + cost, destNode));
                }
            }
        }
        else
        {
            // Traversing the adjacency list
#pragma omp parallel for
            for (int i = 0; i < v.size(); i++)
            {
                int dest = v[i].first;
                int cost = v[i].second;

                // Checking for the cost
                if (d + cost < dis[dest][k - 1])
                {
                    dis[dest][k - 1] = d + cost;

                    // Sorting the distances
                    sort(dis[dest].begin(), dis[dest].end());

// Pushing elements to priority queue
#pragma omp critical
                    {
                        pq.push({(d + cost), dest});
                    }
                }
            }
        }
    }

    // Printing K shortest paths from source to destination
    if (dataset_type == 1)
        cout << "K shortest paths from node " << source << " to node " << dest << ":" << endl;

    for (int i = 0; i < numPaths; i++)
    {
        cout << "Path " << i + 1 << ": " << dis[dest][i] << endl;
    }
}