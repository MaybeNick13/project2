#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <list>
#include <numeric>
#include <ctime>
#include <unordered_map>
#include <list>
#include <set>
#include <queue>
#include <chrono>
#include "utils.h"
#include <map>
#include <cstring>

class search_graph_MRNG{
    private:
    int l_MRNG;
    int N_MRNG;
    int node_popullation;
    int dimen;
    int starting_node;
    MRNG_Node **nodes;
    vector<vector<float>> distances;    // keeping an array with the distances, its usefull and saves time from calculating a distance multiple times
    vector<list<int>> adjacency_list;

    void initialize_distances();

    float get_dist(int point_a, int point_b);

    void resize_multimap(multimap<float, mapNode> starting_map, int size);

    bool check_if_on_map(multimap<float, mapNode> mm, int id);

    public:

    search_graph_MRNG(int l_mrng, int n_mrng, int dimension, int num_of_nodes, MRNG_Node **ns);

    vector<int> search_on_graph(MRNG_Node query);
};

int mrng(int argc, char * argv[]);