#ifndef LSH_H
#define LSH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <chrono>
#include "utils.h"
#include "hashing.h"

using namespace std;

vector<vector<pair_dist_pos>> create_graph(string input, int N);

#endif // LSH_H
