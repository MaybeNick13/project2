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

#include "utils.h"

#include "hashing.h"

#include <cstring>

using namespace std;

vector<vector<pair_dist_pos>> create_graph(string input, int N) {
  string output = "output.txt";
  ofstream outfile;
  outfile.open(output);
  ifstream images(input);
  int k = 4;
  int L = 5;
  if (!images.is_open()) {
    cerr << "Failed to open input.dat" << endl;
  }

  images.seekg(4);
  char buffer[4];
  images.read(buffer, 4);
  NumImages = (static_cast < uint32_t > (static_cast < unsigned char > (buffer[0])) << 24) |
    (static_cast < uint32_t > (static_cast < unsigned char > (buffer[1])) << 16) |
    (static_cast < uint32_t > (static_cast < unsigned char > (buffer[2])) << 8) |
    (static_cast < uint32_t > (static_cast < unsigned char > (buffer[3])));
  int rows, columns;
  images.read(buffer, 4);
  rows = (static_cast < uint32_t > (static_cast < unsigned char > (buffer[0])) << 24) |
    (static_cast < uint32_t > (static_cast < unsigned char > (buffer[1])) << 16) |
    (static_cast < uint32_t > (static_cast < unsigned char > (buffer[2])) << 8) |
    (static_cast < uint32_t > (static_cast < unsigned char > (buffer[3])));
  images.read(buffer, 4);
  columns = (static_cast < uint32_t > (static_cast < unsigned char > (buffer[0])) << 24) |
    (static_cast < uint32_t > (static_cast < unsigned char > (buffer[1])) << 16) |
    (static_cast < uint32_t > (static_cast < unsigned char > (buffer[2])) << 8) |
    (static_cast < uint32_t > (static_cast < unsigned char > (buffer[3])));

  ImageSize = rows * columns;
  bool repeat;
  Node * array = new Node[NumImages];
  for (int i = 0; i < NumImages; i++) {
    images.read(array[i].image.data(), ImageSize);
  }
    repeat = false;
    hashtable tables[L];
    for (int j = 0; j < L; j++) {
      tables[j] = hashtable(j, & array); //arxikopiisi ton hashtables
    }
    vector<vector<pair_dist_pos>> graph (NumImages,vector<pair_dist_pos>(N));
    for (int i = 41360; i < NumImages; i++) {
      if (i % 10000==0){
        cout <<"reached " << i <<endl;
      }
      auto startMethod = chrono::high_resolution_clock::now();
      pair_dist_pos furthest;
      set < int > same_ids; ///gia pithanoun gitones
      priority_queue < pair_dist_pos, vector < pair_dist_pos > , compare > nn_pqueue;
      list < int > candidates;
      set < int > rNeighbors;
      for (int j = 0; j < L; j++) {
        int query_id = array[i].IDS[j];
        int bucket_num = query_id % (NumImages / 4);

        candidates = tables[j].get_bucket(bucket_num); //ikones sto idio bucket
        for (auto itr: candidates) {
          if (array[itr].IDS[j] == query_id) { //idio ID
            same_ids.insert(itr);
          }
        }

      }
      int f = 0;
      for (auto itr: same_ids) {

        float dist = euclidean_distance(array[itr], array[i]); //diasxizoume ton pinaka me ta same_ids
        pair_dist_pos current {
          dist,
          itr
        };
        if (dist!=0){
        if (f < N) { //an den exoume vrei N stoixeia akoma, aplos vale to mesa
          nn_pqueue.push(current);
        } else {
          furthest = nn_pqueue.top();
          if (current.distance < furthest.distance) {
            nn_pqueue.pop();
            nn_pqueue.emplace(current);
          }
        }
        f++;
        }
      }
      auto endMethod = chrono::high_resolution_clock::now();
      chrono::duration < double > durationMethod = endMethod - startMethod;

      for (int j = N - 1; j >= 0; j--) {
        graph[i][j] = nn_pqueue.top();
        nn_pqueue.pop();
      }

     // for (int j = 0; j < N; j++) {
       // outfile << graph[i][j].pos << " ";
      //}
      outfile << endl;
    }
  delete[] array;
  images.close();
  return graph;
}