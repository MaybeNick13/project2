#include "mrng.h"

using namespace std;

void search_graph_MRNG :: initialize_distances(){
    // Ininitializing the distances data structure
    distances = vector<vector<float>>(node_popullation - 1);
    cout << "Initializing distances" << endl;

    for(int i = 0; i < node_popullation - 1; i++){
        cout << i << endl ;
        distances[i] = vector<float> (node_popullation- 1 - i);
        for(int j = i + 1; j < node_popullation; j++){
            distances[i][j - i - 1] = euclidean_distance((*nodes)[i] ,(*nodes)[j]);
        }
    }
}

float search_graph_MRNG :: get_dist(int point_a, int point_b){
    // Given 2 ids of nodes, return their distance
    int big, small;
    if(point_a > point_b){
        big = point_a;
        small = point_b;
    }else if(point_a < point_b){
        big = point_b;
        small = point_a;
    }else if(point_a  == point_b){
        return -1;
    }

    return distances[small][big - small - 1];
}

void search_graph_MRNG :: resize_multimap(multimap<float, mapNode> starting_map, int size){
    if(size >= starting_map.size()){
        return;
    }

    multimap<float, mapNode> dummy_map;
    auto it = starting_map.begin();
    for(int i = 0; i < size; i++){
        dummy_map.insert(*it);
        it++;
    }

    starting_map = dummy_map;
}

bool search_graph_MRNG :: check_if_on_map(multimap<float, mapNode> mm, int id){
    for(const auto &it : mm){
        if(it.second.id == id){
            return true;
        }
    }
    return false;
}

search_graph_MRNG :: search_graph_MRNG(int l_mrng, int n_mrng, int dimension, int num_of_nodes, MRNG_Node **ns) : l_MRNG(l_mrng), N_MRNG(n_mrng), 
node_popullation(num_of_nodes), dimen(dimension), nodes(ns){

    initialize_distances();     // Initializing distances first
    cout << endl << "initializing distances done" << endl;
    adjacency_list = vector<list<int>>(num_of_nodes);
    vector<float> centroid(dimension);

    for(int p = 0; p < node_popullation; p++){
        cout<< p << endl;
        for(int d = 0; d < dimen; d++){
            centroid[d] += (float)(*nodes)[p].image[d];
        }

        multimap <float , int> candidates;
        // polu argo, prepei na ginei me ena vector kai aplws ena sorting sto telos isws na einai pio apodotiko
        for(int i = 0; i < node_popullation; i++){
            if(i == p)
                continue;
            
            candidates.insert({get_dist(p, i), i});

        }
        
        adjacency_list[p].push_back(candidates.begin()->second);
        candidates.erase(candidates.begin()->first);

        bool condition = true;

        for(auto n = candidates.begin(); n != candidates.end(); n++){

            bool condition = true;

            for(auto t = adjacency_list[p].begin(); t != adjacency_list[p].end(); t++){

                if(get_dist(*t, p) > get_dist(*t, n->second) && get_dist(*t, p) > get_dist(n->second, p)){
                    condition = false;
                    break;
                    // etsi opws exei graftei o algori9mos einai poly argo pali (n^2*d*neighbors) 
                    // alla den nomizw oti mporei na apofeux9ei auto dusthxws
                }
            }
            if(condition == true){
                adjacency_list[p].push_back(n->second);
            }

        }

    }


    for(int d = 0; d < dimen; d++){
        centroid[d] = centroid[d] / num_of_nodes;    
    }

    //finding nearest node to centroid of entire dataset, that is our search node
    double min_dist = euclidean_distance((*nodes)[0], centroid);
    int position = 0;

    for(int p = 1; p < num_of_nodes; p++){
        float p_dist = euclidean_distance((*nodes)[p], centroid);
        if(p_dist < min_dist){
            position = p;
            min_dist = p_dist;
        }
    }

    starting_node = position;

}

vector<int> search_graph_MRNG :: search_on_graph(MRNG_Node query){
    
    multimap<float , mapNode> S;
    vector<int> knn(N_MRNG);
    mapNode st_node;
    st_node.checked = false;
    st_node.id = starting_node;
    S.insert({euclidean_distance(query, (*nodes)[starting_node]), st_node});

    bool stop = false;
    while(true){
        int next_node = -1;
        for(auto &nd : S){
            if(nd.second.checked == false){
                nd.second.checked = true;
                next_node = nd.second.id;
            }
        }

        if(next_node == -1){
            break;    
        }

        for(auto &neighbor : adjacency_list[next_node]){
            if(check_if_on_map(S, neighbor) == true)
                continue;
            mapNode dummynd;
            dummynd.checked = false;
            dummynd.id = neighbor;
            S.insert({euclidean_distance((*nodes)[neighbor], query), dummynd});
        }

        resize_multimap(S, l_MRNG);
    }

    auto it = S.begin();
    for(int i = 0; i < N_MRNG; i++){
        knn[i] = (*it).second.id;
        it++;
    }

    return knn;
}


int mrng(int argc, char * argv[]){
    int N = 5;
    int l = 5; 
    char* input_name, *query_name, *output_name;
    int dimensions, num_of_images;
    ifstream in_str, q_str;
    ofstream out_str;

    input_name = get_str_of_option(argv, argv + argc, "-d");

    if(input_name)
		in_str = ifstream(input_name);
	else{
		cout << "Please provide input file next time, the program will now terminate" << endl;
		exit(-1);
	}

    query_name = get_str_of_option(argv, argv + argc, "-q");

    if(query_name)
		q_str = ifstream(query_name);
	else{
		cout << "Please provide input file next time, the program will now terminate" << endl;
		exit(-1);
	}

    output_name = get_str_of_option(argv, argv + argc, "-o");

	if(output_name)
		out_str = ofstream(output_name);
	else{
		cout << "Please provide output file name next time, the program will now terminate" << endl;
		exit(-2);
	}

    if (!in_str.is_open()) {
		cerr << "Failed to open input file" << endl;
        cout << "File name:" << input_name << endl;
		exit(errno);
	}

    if (!q_str.is_open()) {
		cerr << "Failed to open query file" << endl;
		exit(errno);
	}
    char* N_str, *l_str;

    N_str = get_str_of_option(argv, argv + argc, "-N");
    if(N_str)
        N = atoi(N_str);

    l_str = get_str_of_option(argv, argv + argc, "-l");
    if(l_str) 
        l = atoi(l_str);
    in_str.seekg(4);
    char buffer[4];
    in_str.read(buffer,4);
    num_of_images = (static_cast<uint32_t>(static_cast<unsigned char>(buffer[0]))<< 24) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[1])) << 16) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[2])) << 8) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[3])) );
    num_of_images = 1000;
    int rows, columns;
    in_str.read(buffer,4);
    rows = (static_cast<uint32_t>(static_cast<unsigned char>(buffer[0]))<< 24) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[1])) << 16) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[2])) << 8) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[3])) );
    in_str.read(buffer,4);
    columns = (static_cast<uint32_t>(static_cast<unsigned char>(buffer[0]))<< 24) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[1])) << 16) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[2])) << 8) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[3])) );

    dimensions = rows * columns;

    ImageSize = dimensions;
    
    MRNG_Node * array = new MRNG_Node[num_of_images]; //DONT FORGET TO DELETE

	for (int i = 0; i < num_of_images - 1; i++) {
		in_str.read((char*)(array[i].image.data()), dimensions);
	}
    auto constructGraph = chrono::high_resolution_clock::now();
    search_graph_MRNG graph(l, N, dimensions, num_of_images, &array);
    auto constructGraphEnd = chrono::high_resolution_clock::now();
    chrono::duration < double > graphTime = constructGraphEnd - constructGraph;
    double seconds= graphTime.count();


    cout << "SEARCH GRAPH DONE in " << seconds <<" seconds" << endl;

    q_str.seekg(4);
    char q_buffer[4];
    q_str.read(q_buffer,4);
    int num_of_queries, q_dimen;
    num_of_queries = (static_cast<uint32_t>(static_cast<unsigned char>(q_buffer[0]))<< 24) |
    (static_cast<uint32_t>(static_cast<unsigned char>(q_buffer[1])) << 16) |
    (static_cast<uint32_t>(static_cast<unsigned char>(q_buffer[2])) << 8) |
    (static_cast<uint32_t>(static_cast<unsigned char>(q_buffer[3])) );
    int q_rows, q_columns;
    q_str.read(q_buffer,4);
    q_rows = (static_cast<uint32_t>(static_cast<unsigned char>(buffer[0]))<< 24) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[1])) << 16) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[2])) << 8) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[3])) );
    q_str.read(buffer,4);
    q_columns = (static_cast<uint32_t>(static_cast<unsigned char>(buffer[0]))<< 24) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[1])) << 16) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[2])) << 8) |
    (static_cast<uint32_t>(static_cast<unsigned char>(buffer[3])) );

    q_dimen = q_rows * q_columns;
    if(q_dimen != dimensions){
        cout << "Query dimensions not same as input data dimensions, program will now terminate" << endl;
        exit(-3);
    }

    MRNG_Node * queries = new MRNG_Node[num_of_queries];

    for (int i = 0; i < num_of_queries - 1; i++) {
		in_str.read((char*)(queries[i].image.data()), dimensions);
	}

    for(int q = 0; q < 10; q++){
        vector<int> NNN;
        NNN = graph.search_on_graph(queries[q]);
        out_str << "Query " << q << " N nearest neighbors:" << endl;
        int i = 1;
        for(auto itr : NNN){
            out_str << "Neigbor " <<i << ": " << itr << ", distance :" << euclidean_distance(array[itr], queries[q]) << endl;
            i++;
        }
        int min = 0;
        float min_pos = euclidean_distance(queries[q], array[0]);
        for(int i = 1; i < num_of_images; i++){
            float dist = euclidean_distance(queries[q], array[i]);
            if(dist < min_pos){
                min = i;
                min_pos = dist;
            }
        }
        out_str << "Actuall NN: " << min_pos << " in position: " << min << endl;
    }
    delete[] queries;
    delete[] array;
    return 0;
}

