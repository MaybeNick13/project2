#include "hashing.h"


hashfunc::hashfunc() {}

hashfunc::hashfunc(Node* nodes) {
    random_device rd{};
    mt19937 gen{rd()};
    normal_distribution<float> myRand(0.5, 0.25);

    proj.resize(ImageSize);
    float length = 0;
    for (int i = 0; i < ImageSize; i++) {
        proj[i] = myRand(gen); // vector initialization
        length += proj[i] * proj[i];
    }
    length = sqrt(length);

    for (int i = 0; i < ImageSize; i++) {

        proj[i] = proj[i] / length; // kanonikopoihsh
    }

    uniform_real_distribution<float> myRealRand(0.0, W - 0.00000001);
    t = myRealRand(gen); 

    hashvalues = vector<int>(NumImages);

    for (int i = 0; i < NumImages; i++) {
        float innerprod = 0;
        for (int j = 0; j < ImageSize; j++) {
            innerprod += proj[j] * (float)(static_cast<unsigned char>(nodes[i].image[j]));
        }

        hashvalues[i] = (innerprod + t) / W;
    }
}

float hashfunc::hash(Node node) {        //function gia to hashing to query meta tin arxikopiisi olou tou table
    float innerprod = 0;
    for (int j = 0; j < ImageSize; j++) {
        innerprod += proj[j] * (float)(static_cast<unsigned char>(node.image[j]));
    }
    return ((innerprod + t) / W);
}

float hashfunc::hash(vector<float> center) {
    float innerprod = 0;
    for (int j = 0; j < ImageSize; j++) {
        innerprod += proj[j] * center[j];
    }
    return ((innerprod + t) / (float)W);
}

vector<int> hashfunc::get_values() {
    return hashvalues;
}

hashtable::hashtable() {}

hashtable::hashtable(int num, Node** im) : images(im), id_num(num) {
    vector<vector<int>> hval(k);
    int factors[k];
    srand((unsigned)time(NULL));
    int random = rand();
    int ID [NumImages];
    int gHash [NumImages];

    for (int i = 0; i < k; i++) {                   //ftiaxnei k hashfuncs kai apothikevi tis times tous
        hashfunc val(*images);
        hval[i] = val.get_values();
        factors[i] = rand() % 9999999+11;
        hashfuncs.push_back(val);
    }

    for (int i = 0; i < NumImages; i++) {
        ID[i] = 0;
    }

    for (int i = 0; i < NumImages; i++) {
        for (int j = 0; j < k; j++) {
            ID[i] +=factors[j] *hval[j][i];
        }
        ID[i] = ID[i] % M;
        (*images)[i].IDS[id_num] = ID[i];
    }

    for (int i = 0; i < NumImages; i++) {
        gHash[i] = ID[i] % (NumImages / 4);
        map.insert(gHash[i], i);
    }
}

void hashtable::hash(Node* im) {
    vector<int> hval(k);
    int ID = 0;
    for (int i = 0; i < k; i++) {
        ID += hashfuncs[i].hash(*im);
    }
    ID = ID % M;
    im->IDS[id_num] = ID;
}

void hashtable::hash(vector<float> centroid, vector<int> *IDS){
    vector<int> hval(k);
    int ID = 0;
    for (int i = 0; i < k; i++) {
        ID += hashfuncs[i].hash(centroid);
    }
    ID = ID % M;
    (*IDS)[id_num] = ID;
}

list<int> hashtable::get_bucket(int bucket_num) {
    return map.get(bucket_num);
}
