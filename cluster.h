#ifndef CLUSTER_H
#define CLUSTER_H
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
#include "hypercube.h"
#include <algorithm>
#include <cstring>
#include <iomanip>

using namespace std;


class cluster{
	private:
	vector<vector<float>> centroids;
	vector<vector<float>> sum_of_pts;
	vector<set <int>> groups;
	vector<int> group_num;
	Node** points;
	int num_of_centroids;
	int num_of_points;
	int dimen;

	void insert_to_grp(int grp_num, int point_num);

	bool remove_from_group(int grp_num , int point_num);	

	bool check_conv(vector<vector <float>> old);

	public:
	cluster(Node** pts, int nc, int np, int dim);

	void assign_lloyds();

	void assign_lsh_range();

	void assign_hypercube_range();

	vector<float> compute_silhouette();

	float avg_dist_clust_pts(int clust_num, int point_num);

	void update_2_centers(int in, int out);

	void update_centers();

	vector<float> get_centroid(int c);

	set <int> get_group(int c);

	int sizeof_group(int c);
};

#endif