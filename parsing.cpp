#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main (int argc, char* argv[]){
    cout <<"check1"<<endl;
    ifstream images;
    string line;
    images.open(argv[1]);
    for (int i=0; i<10; i++){
        getline (images,line);
        cout << line << '\n';
    }
    images.close();
}