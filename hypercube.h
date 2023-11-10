#ifndef HYPERCUBE_H
#define HYPERCUBE_H
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <numeric>
#include <ctime>
#include <unordered_map>
#include <list>
#include <set>
#include <queue>
#include <chrono>
#include <bitset>
#include <cstring>
#include "utils.h"
#include "hashing.h"


class biMap{
    private:
        unordered_map<int,bool> prevRes;
        default_random_engine boolGen;
        bool myRand();
    public:
        biMap();
        bool mapper(int value);
};

class hyperCube{
    public:
    vector<string> edges;
        hyperCube();
        hyperCube(Node *array);
};

#endif