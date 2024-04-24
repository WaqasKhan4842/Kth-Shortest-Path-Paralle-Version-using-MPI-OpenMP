#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

class GraphProcessor {
private:
    std::vector<std::vector<std::pair<int, int> > > Graph;
    int numEdges;
    int numNodes;

    std::pair<int, int> chooseRandomEdge();

public:
    GraphProcessor();
    void initializeGlobalGraph(int n);
    void addEdgeToGlobalGraph(int u, int v, int w);
    void read_file_DoctorWho(int sub_choice);
    void read_file_SNAP(int sub_choice);
};


