#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <utility> // for std::pair
#include <unordered_map>

class GraphProcessor
{
private:
    std::vector<std::vector<std::pair<int, int>>> Graph;
    std::vector<std::pair<int, int>> randomEdges;
    std::unordered_map<std::string, int> nodeIndices;
    std::unordered_map<int, std::string> nodeNames;

    int numEdges;
    int numNodes;

public:
    GraphProcessor();
    void initializeGlobalGraph(int n);
    void addEdgeToGlobalGraph(int u, int v, int w);
    void read_file_DoctorWho(int sub_choice);
    void read_file_SNAP(int sub_choice);
    void read_file_SNAP_Parallel(int sub_choice);
    std::pair<int, int> chooseRandomEdge();
    void serial_execution(int dataset_type);
    void parallel_execution(int process_rank, int comm_size, int dataset_type);
};