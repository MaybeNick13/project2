#include "lsh.h"

#include "utils.h"

#include <iostream>

#include <fstream>

#include <vector>

#include <random>

#include <cstring>

using namespace std;

int main(int argc, char * argv[]) {
  string input = "input.dat";
  string queryF = "query.dat";
  string output = "graphRes.txt";
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
      N = std::stoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-E") == 0) {
      E = std::stoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-N") == 0) {
      GraphN = std::stoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-R") == 0) {
      R = std::stoi(argv[i + 1]);
    }
  }

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

  Node * array = new Node[NumImages+1];

  for (int i = 0; i < NumImages; i++) {
    images.read(array[i].image.data(), ImageSize);
  }
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> distribution(0, NumImages - 1);
  vector<vector<pair_dist_pos>> graph = create_graph(input,N);
  pair_dist_pos * methodResult = new pair_dist_pos[GraphN];
   pair_dist_pos * exhaustiveResult  = new pair_dist_pos[GraphN];
  bool repeat=false;
    do {
    repeat = false;
    double totalMethodDuration = 0.0;
    double totalExhaustiveDuration = 0.0;
    double maf=-1;
    ifstream query(queryF);
    if (!query.is_open()) {
      cerr << "Failed to open query.dat" << endl;
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
    for (int i=0; i<queryImages; i++){
        array[NumImages]=queries[i];
        auto startMethod = chrono::high_resolution_clock::now();
        priority_queue < pair_dist_pos, vector < pair_dist_pos > , compare > nn_pqueue;
        for (int j=0; j < R;j++){
            int curr= distribution(gen);
            for (int t=0;t<50; t++){
                vector <pair_dist_pos> fromQuery (GraphN);
                float distFromQuery= 9999999999;
                int next;
                for (int z=0; z<E; z++){
                    pair_dist_pos buff;
                    buff.pos=graph[curr][z].pos;
                    buff.distance=euclidean_distance(array[buff.pos],queries[i]);
                    if (buff.distance < distFromQuery){
                        distFromQuery=buff.distance;
                        next=buff.pos;
                    }
                    nn_pqueue.emplace(buff);
                }
                curr=next;
            }
        }
        auto endMethod = chrono::high_resolution_clock::now();
        chrono::duration < double > durationMethod = endMethod - startMethod;  
        totalMethodDuration+= durationMethod.count();
        auto startExhaustive = chrono::high_resolution_clock::now();
         priority_queue<pair_dist_pos, vector<pair_dist_pos>, compare> distances = calculateDistances(array, NumImages);
        auto endExhaustive = chrono::high_resolution_clock::now();
        chrono::duration < double > durationExhaustive = endExhaustive - startExhaustive;
        totalExhaustiveDuration+=durationExhaustive.count();
        for (int j = GraphN - 1; j >= 0; j--) {
            exhaustiveResult[j] = distances.top();
            distances.pop();
            methodResult[j] = nn_pqueue.top();
            nn_pqueue.pop();
            if ((methodResult[j].distance/exhaustiveResult[j].distance) > maf ){
                maf=methodResult[j].distance/exhaustiveResult[j].distance;
            }
      }  
      outfile <<"Query" << i <<endl;
        for (int j = 0; j < GraphN; j++) {
        outfile << "Nearest neighbor-" << j + 1 << " " << methodResult[j].pos << endl;
        outfile << "distanceApproximate:" << methodResult[j].distance << endl;
        outfile << "distanceTrue:" << exhaustiveResult[j].distance << endl;
      }

    }
      outfile << "tAverageApproximate:" << (totalMethodDuration/queryImages) << endl;
      outfile << "tAverageTrue:" << (totalExhaustiveDuration/queryImages) << endl;
      outfile <<"MAF"<< maf << endl;
    cout << "Repeat with different query?[y/n]" << endl;
    string answer;
    cin >> answer;
    if (answer.compare("y") == 0) {
      cout << "Enter query file name" << endl;
      cin >> answer;
      repeat = true;
      queryF = answer;
    }
}while (repeat == 1);
  delete[] array;
  images.close();
  delete[] methodResult;
  delete[] exhaustiveResult;

  return 0;
}

