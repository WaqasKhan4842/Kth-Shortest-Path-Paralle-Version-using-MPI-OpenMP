#include "GraphProcessor.h"
#include "Serial_KShortest.h" // Include any other necessary headers

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
    unordered_map<string, int> nodeIndices;
    unordered_map<int, string> nodeNames;   // Map to store index -> node name
    string filename, line, source, target, weight, type;
    int numEdges = 0;

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

    int numNodes = nodeIndices.size();
    cout << "Number of nodes: " << numNodes << endl;
    cout << "Number of edges: " << numEdges << endl;

    for(int i=0;i<5;i++)
    {
    pair<int, int> randomEdge = chooseRandomEdge();
    string randomSource = nodeNames[randomEdge.first];
    string randomDest = nodeNames[randomEdge.second];

    // Print the statement
    cout << "K shortest paths from node " << randomSource << " to node " << randomDest << ":" << endl;
    findKShortest(Graph, numEdges, numNodes, 4, randomEdge.first, randomEdge.second);
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
    cout << "No of EDGES: " << numEdges << endl;
    // printGraph();
    pair<int, int> randomEdge = chooseRandomEdge();
    findKShortest(Graph, numEdges, numNodes, 10, randomEdge.first, randomEdge.second);
}
