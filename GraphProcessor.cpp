#include "GraphProcessor.h"
#include "Serial_KShortest.h" // Include any other necessary headers
#include <mpi.h>
using namespace std;

GraphProcessor::GraphProcessor() : numEdges(0), numNodes(0) {}

void GraphProcessor::initializeGlobalGraph(int n)
{
    Graph.resize(n + 1);
}

void GraphProcessor::addEdgeToGlobalGraph(int u, int v, int w)
{
    Graph[u].push_back(make_pair(v, w));
}

pair<int, int> GraphProcessor::chooseRandomEdge()
{
    int randomIndex = rand() % Graph.size();
    while (Graph[randomIndex].empty())
    {
        randomIndex = rand() % Graph.size();
    }
    int randomEdgeIndex = rand() % Graph[randomIndex].size();
    return make_pair(randomIndex, Graph[randomIndex][randomEdgeIndex].first);
}

void GraphProcessor::read_file_DoctorWho(int sub_choice)
{
    // Initialize Graph
    int nodeCount = 0;

    string filename, line, source, target, weight, type;
    numEdges = 0;

    if (sub_choice == 1)
        filename = "DataSets/classic-who.csv";
    else if (sub_choice == 2)
        filename = "DataSets/doctorwho.csv";
    else
        filename = "DataSets/new-who.csv";

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file.\n";
        exit(1);
    }
    getline(file, line);
    while (getline(file, line))
    {
        stringstream ss(line);
        getline(ss, source, ',');
        getline(ss, target, ',');
        getline(ss, weight, ',');
        getline(ss, type, ',');

        if (nodeIndices.find(source) == nodeIndices.end())
        {
            nodeIndices[source] = nodeCount++;
            nodeNames[nodeIndices[source]] = source; // Store the node name corresponding to its index
            Graph.emplace_back();
        }
        if (nodeIndices.find(target) == nodeIndices.end())
        {
            nodeIndices[target] = nodeCount++;
            nodeNames[nodeIndices[target]] = target; // Store the node name corresponding to its index
            Graph.emplace_back();
        }
        int weightVal = stoi(weight);
        Graph[nodeIndices[source]].push_back(make_pair(nodeIndices[target], weightVal));
        Graph[nodeIndices[target]].push_back(make_pair(nodeIndices[source], weightVal));
        ++numEdges;
    }

    numNodes = nodeIndices.size();
    cout << "Number of nodes: " << numNodes << endl;
    cout << "Number of edges: " << numEdges << endl;

    for (int i = 0; i < 10; ++i)
    {
        randomEdges.push_back(chooseRandomEdge());
    }
}

void GraphProcessor::read_file_SNAP(int sub_choice)
{
    string filename;
    if (sub_choice == 1)
        filename = "DataSets/email-Enron.txt";
    else
        filename = "DataSets/email-EuAll.txt";

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file.\n";
        return;
    }
    int numEdgesFound = 0;
    string line;
    while (getline(file, line))
    {
        if (line[0] != '#')
            break;

        if (!numEdgesFound && sscanf(line.c_str(), "# Nodes: %d Edges: %d", &numNodes, &numEdges) == 2)
        {
            cout << "Number of Edges : " << numEdges << endl;
            numEdgesFound = 1;
        }
    }
    initializeGlobalGraph(numEdges + 1);
    srand(time(NULL));
    for (int i = 0; i < numEdges; i++)
    {
        int u, v, w;
        if (!(file >> u >> v))
        {
            cerr << "Error reading edge " << i + 1 << ".\n";
            continue;
        }
        w = 1;
        addEdgeToGlobalGraph(u, v, w);
    }

    // cout << "No of EDGES: " << numEdges << endl;
    //  printGraph();
    //  Vector to store random edges

    // Generate 10 random edges
    for (int i = 0; i < 10; ++i)
    {
        randomEdges.push_back(chooseRandomEdge());
    }
}
void GraphProcessor::serial_execution(int dataset_type)
{
    if (dataset_type == 1)
    {
        for (int i = 0; i < 10; ++i)
        {
            findKShortest(Graph, numEdges, numNodes, 2, randomEdges[i].first, randomEdges[i].second, dataset_type, 1);
        }
    }
    else
    {
        for (int i = 0; i < 10; ++i)
        {
            string randomSource = nodeNames[randomEdges[i].first];
            string randomDest = nodeNames[randomEdges[i].second];

            // Print the statement
            cout << "K shortest paths from node " << randomSource << " to node " << randomDest << ":" << endl;
            findKShortest(Graph, numEdges, numNodes, 2, randomEdges[i].first, randomEdges[i].second, dataset_type, 1);
        }
    }
}
void GraphProcessor::parallel_execution(int process_rank, int comm_size, int dataset_type)
{
    int num_edges_per_process = randomEdges.size() / comm_size;
    int remainder = randomEdges.size() % comm_size;

    // Calculate the number of elements each process will receive
    int recv_count = (process_rank < remainder) ? (num_edges_per_process + 1) : num_edges_per_process;

    // Allocate space to receive the portion of randomEdges for this process
    std::vector<std::pair<int, int>> local_edges(recv_count);

    // Scatter randomEdges to all processes
    MPI_Scatter(randomEdges.data(), recv_count * 2, MPI_INT,
                local_edges.data(), recv_count * 2, MPI_INT, 0, MPI_COMM_WORLD);
    if (dataset_type == 1)
    {
        for (const auto &edge : local_edges)
        {
            findKShortest(Graph, numEdges, numNodes, 2, edge.first, edge.second, dataset_type, 2);
        }
    }
    else
    {

        for (const auto &edge : local_edges)
        {

            string randomSource = nodeNames[edge.first];
            string randomDest = nodeNames[edge.second];

            // Print the statement
            cout << "K shortest paths from node " << randomSource << " to node " << randomDest << ":" << endl;
            findKShortest(Graph, numEdges, numNodes, 2, edge.first, edge.second, dataset_type, 2);
        }
    }
}