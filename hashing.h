#ifndef HASHING_H
#define HASHING_H
#include "utils.h"
#include <vector>
#include <list>
#include <random>
#include <cmath>
#include <ctime>
#include <map>

// Forward declaration of the Node class to resolve the circular dependency
class Node;

// Class definition for hashfunc
class hashfunc {
private:
    std::vector<int> hashvalues;
    std::vector<float> proj;
    float t;

public:
    hashfunc();
    hashfunc(Node* nodes);
    float hash(Node node);
    std::vector<int> get_values();
    float hash(vector<float> node);


};

// Class definition for hashtable
class hashtable {
private:
    Node** images;
    int id_num;
    myMap map;
    std::vector<hashfunc> hashfuncs;
    int* factors;

public:
    hashtable();
    hashtable(int num, Node** im);
    void hash(Node* im);
    void hash(vector<float> centroid, vector<int> *IDS);
    std::list<int> get_bucket(int bucket_num);
};

#endif
