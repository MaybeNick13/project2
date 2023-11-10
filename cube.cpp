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
  string output = "output.txt";
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
  for (int i = 0; i < NumImages - 1; i++) {
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
  pair_dist_pos * exhaustiveResult = new pair_dist_pos[N];
  bool repeat;
  do {
    repeat = false;
    ifstream query(queryF);
    if (!query.is_open()) {
      cerr << "Failed to open query file" << endl;
      return 1;
    }
    query.seekg(4);
    int queryImages;
    query.read(buffer, 4);
    queryImages = (static_cast < uint32_t > (static_cast < unsigned char > (buffer[0])) << 24) |
      (static_cast < uint32_t > (static_cast < unsigned char > (buffer[1])) << 16) |
      (static_cast < uint32_t > (static_cast < unsigned char > (buffer[2])) << 8) |
      (static_cast < uint32_t > (static_cast < unsigned char > (buffer[3])));
    if (queryImages > 10) {
      queryImages = 10;
    }
    query.seekg(16);
    Node queries[queryImages];
    for (int i = 0; i < queryImages; i++) {
      query.read(queries[i].image.data(), ImageSize);
    }
    for (int i = 0; i < queryImages; i++) {

      auto startMethod = chrono::high_resolution_clock::now();
      priority_queue < pair_dist_pos, vector < pair_dist_pos > , compare > pq;
      int query_pos = NumImages - 1;
      array[query_pos].image = queries[i].image;
      vector < int > queryIDS(ck);
      for (int j = 0; j < ck; j++) {
        queryIDS[j] = biFunc.mapper(h[j].hash(array[query_pos])); //vriskoume ti thesi kai gia to query
      }
      string sedge = to_string(queryIDS[0]);
      for (int j = 1; j < ck; j++) {
        sedge = (sedge) + to_string(queryIDS[j]); //sinthetoume to arxiko edge tou query
      }
      int f = 0;
      pair_dist_pos furthest;
      vector < string > edges;
      edges.push_back(sedge);
      for (int j = 1; j < cProbes; j++) { //vriskoume ta alla edges pou isos xriastoume sto NN kai sigoura tha xriastoume sto R-range search
        string edgeBuff = sedge;
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
          if (edges[j].compare(cube.edges[v]) == 0) {
            float dist = euclidean_distance(array[v], array[query_pos]);
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
      auto startExhaustive = chrono::high_resolution_clock::now();
      priority_queue < pair_dist_pos, vector < pair_dist_pos > , compare > distances = calculateDistances(array, query_pos);
      auto endExhaustive = chrono::high_resolution_clock::now();
      chrono::duration < double > durationExhaustive = endExhaustive - startExhaustive;
      for (int j = N - 1; j >= 0; j--) {
        exhaustiveResult[j] = distances.top();
        distances.pop();
        methodResult[j] = pq.top();
        pq.pop();
      }
      set < int > rNeighbors; //EDO TO RANGE SEARCH
      for (int j = 0; j < cProbes; j++) {
        for (int v = 0; v < NumImages; v++) {
          if (edges[j].compare(cube.edges[v]) == 0) {
            float dist = euclidean_distance(array[v], array[query_pos]);
            pair_dist_pos current {
              dist,
              v
            };
            if (dist < R) {
              rNeighbors.insert(v);
            }
          }
        }
      }
      //
      outfile << fixed;
      for (int j = 0; j < N; j++) {
        outfile << "Nearest neighbor-" << j + 1 << ": " << methodResult[j].pos << endl;
        outfile << "distanceHypercube:" << euclidean_distance(array[query_pos], array[methodResult[j].pos]) << endl;
        outfile << "distanceTrue:" << exhaustiveResult[j].distance << endl;
      }
      outfile << "tHypercube:" << durationMethod.count() << endl;
      outfile << "tTrue:" << durationExhaustive.count() << endl;
      outfile << "R-near neighbors:" << endl;
      for (const auto & element: rNeighbors) {
        outfile << element << endl;
      }

    }
    cout << "Repeat with different query?[y/n]" << endl;
    string answer;
    cin >> answer;
    if (answer.compare("y") == 0) {
      cout << "Enter query file name" << endl;
      cin >> answer;
      repeat = true;
      queryF = answer;
    }
  } while (repeat == 1);
  outfile.close();
  images.close();
  delete[] array;
  delete[] methodResult;
  delete[] exhaustiveResult;
  return 0;
}