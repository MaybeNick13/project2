#include "hypercube.h"


bool biMap::myRand(){
    uniform_int_distribution<int> dist(0,1);
    return dist(boolGen)==1;
}

biMap::biMap() {
    random_device rd;
    boolGen.seed(rd());
}

bool biMap::mapper(int value){
    if (prevRes.find(value)!= prevRes.end()){
        return prevRes[value];
    }
    else {
        prevRes[value]= myRand();
        return prevRes[value];
    }
}



hyperCube::hyperCube(){}

hyperCube::hyperCube(Node *array){
    edges.resize(NumImages + 1);
    for (int i=0; i<NumImages;i++){
        edges[i] =to_string(array[i].IDS[0]);
        for (int j=1; j < ck ; j++){
            edges[i] = (edges[i]) + to_string(array[i].IDS[j]);
        }
    }
}
