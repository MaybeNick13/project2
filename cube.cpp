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

#include "hypercube.h"

using namespace std;

int main(int argc, char * argv[]) {
  string input = "input.dat";
  string queryF = "query.dat";
  string output = "graphcube.txt";
  for (int i = 1; i < argc; i++) { //parsing gia ta input variables
    if (strcmp(argv[i], "-o") == 0) {
      output = argv[i + 1];
    }
    if (strcmp(argv[i], "-d") == 0) {
      input = argv[i + 1];
    }
    if (strcmp(argv[i], "-q") == 0) {
      queryF = argv[i + 1];
    }
    if (strcmp(argv[i], "-k") == 0) {
      ck = std::stoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-M") == 0) {
      cM = std::stoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-N") == 0) {
      N = std::stoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-R") == 0) {
      R = std::stoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-probes") == 0) {
      cProbes = std::stoi(argv[i + 1]);
    }
  }

  L = ck;
  ifstream images(input);
  ofstream outfile;
  outfile.open(output);
  if (!images.is_open()) {
    cerr << "Failed to open input file" << endl;
    return 1;
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

  Node * array = new Node[NumImages];
  for (int i = 0; i < NumImages; i++) {
    images.read(array[i].image.data(), ImageSize);
  }

  vector < int > hval;
  biMap biFunc;
  hashfunc h[ck];
  for (int i = 0; i < ck; i++) {
    h[i] = hashfunc(array);
    hval = h[i].get_values();
    for (int j = 0; j < NumImages; j++) {
      array[j].IDS[i] = biFunc.mapper(hval[j]); // edo ta IDS ine an to i psifio tou edge identifier ine 0 i 1
    }
  }
  hyperCube cube = hyperCube(array);
  pair_dist_pos * methodResult = new pair_dist_pos[N];
    for (int i = 0; i < NumImages; i++) {

      auto startMethod = chrono::high_resolution_clock::now();
      priority_queue < pair_dist_pos, vector < pair_dist_pos > , compare > pq;
      int f = 0;
      pair_dist_pos furthest;
      vector < string > edges;
      edges.push_back(cube.edges[i]);
      for (int j = 1; j < cProbes; j++) { //vriskoume ta alla edges pou isos xriastoume sto NN kai sigoura tha xriastoume sto R-range search
        string edgeBuff = cube.edges[i];
        if (edgeBuff[j - 1] == '0') {
          edgeBuff[j - 1] = '1';
        } else {
          edgeBuff[j - 1] = '0';
        }
        edges.push_back(edgeBuff);
      }
      for (int j = 0; j < cProbes; j++) {
        if (f == cM) {
          break;
        }
        for (int v = 0; v < NumImages; v++) {
          if (edges[j].compare(cube.edges[v]) == 0 && i!=v) {
            float dist = euclidean_distance(array[v], array[i]);
            pair_dist_pos current {
              dist,
              v
            };
            if (f < N) {
              pq.push(current);
            } else {
              furthest = pq.top();
              if (current.distance < furthest.distance) {
                pq.pop();
                pq.emplace(current);
              }
            }
            f++;
            if (f == cM) {
              break;
            }
          }
        }
      }
      auto endMethod = chrono::high_resolution_clock::now();
      chrono::duration < double > durationMethod = endMethod - startMethod;
      for (int j = N - 1; j >= 0; j--) {
        methodResult[j] = pq.top();
        pq.pop();
      }
      for (int j = 0; j < N; j++) {
        outfile << methodResult[j].pos << " ";
      }
      outfile << endl;
    }
  outfile.close();
  images.close();
  delete[] array;
  delete[] methodResult;
  return 0;
}