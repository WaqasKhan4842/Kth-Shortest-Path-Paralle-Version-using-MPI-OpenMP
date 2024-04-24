#include<iostream>
#include<vector>
#include <queue>
using namespace std;

 
// Function to find K shortest path lengths
void findKShortest(const vector<vector<pair<int, int> > >& g, int n, int m, int k, int source, int dest)
{
    // Vector to store distances
    vector<vector<int> > dis(n + 1, vector<int>(k, 1e9));

    // Initialization of priority queue
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push(make_pair(0, source));
    dis[source][0] = 0;

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
        vector<pair<int, int> > v = g[u];

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
    cout << "K shortest paths from node " << source << " to node " << dest << ":" << endl;
    int numPaths = min(k, m); // Number of paths found cannot exceed the number of edges
    for (int i = 0; i < numPaths; i++)
    {
        cout << "Path " << i + 1 << ": " << dis[dest][i] << endl;
    }
}
