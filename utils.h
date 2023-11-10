#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <list>
#include <iostream>
#include <cmath>
#include <queue>
#include <algorithm>
#include <cstring>
#include <iomanip>

using namespace std;
extern int W;
extern int k;
extern int L;
extern int N;
extern int R;
extern int NumImages;
extern int ImageSize;
extern unsigned long M;
extern int ck;
extern int cM;
extern int cProbes;

struct kmean_node{
	int pos;
	float key;
};

struct myPair {
    int key;
    int pos;
};

struct myfPair {
    unsigned long key;
    int pos;
};

class myMap {
private:
    std::vector<std::list<myPair>> table;

public:
    myMap();
    void insert(int key, int value);
    std::list<int> get(int key);
    void display();
};
class myfMap {
private:
    std::vector<std::list<myfPair>> table;

public:
    myfMap();
    void insert(unsigned long key, int value);
    std::list<int> get(unsigned long key);
    void display();
};

class Node {
public:
    std::vector<char> image;
    std::vector<int> IDS;
    Node();
};

struct pair_dist_pos {
    float distance;
    int pos;
};

struct compare {
    bool operator()(const pair_dist_pos &a, const pair_dist_pos &b);
};

int findSmallestGreater(const vector<kmean_node>& vec, int n);

float euclidean_distance(const Node &a, const Node &b);

float euclidean_distance(const vector<float> &a, const vector<float> &b);

float euclidean_distance(const Node& a, const vector<float> &b);

float sqr_euclidean_distance_points(const vector<char> &a, const vector<float> &b);

int vector2int(vector<int> IDS);

char* get_str_of_option(char** begin, char** end, const string& option);

bool check_if_option_exists(char** begin, char** end, const string& option);

int get_num_from_line(string line, int del_pos);


priority_queue<pair_dist_pos, vector<pair_dist_pos>, compare> calculateDistances(Node* array, int query_pos);

#endif
