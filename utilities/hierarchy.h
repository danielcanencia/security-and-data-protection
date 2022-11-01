#ifndef _HIERARCHY_H
#define _HIERARCHY_H


#include <iostream>
#include <vector>
#include <fstream>	// ifstream
#include <filesystem>   // filesystem utilities
#include <string.h>	// strcasecmp
#include <iterator>	// iterator utilities
#include <cctype>	// transform and find
#include <algorithm>	// transform and find
#include <map>

using namespace std;
namespace fs = std::filesystem;

map<int, vector<vector<string>>> read_directory(
	fs::path const &directory,
	vector<vector<string>>& dataset,
  	vector<string>& headersVector,
	const int K,
	vector<string> attQids,
	vector<int>& qids);
vector<vector<string>> transpose(
	const vector<vector<string>>& matrix);
vector<vector<int>> getPermutations(int r,
	const vector<int> nodeMax);
void permute(const vector<int> data,
             vector<vector<int>>& permutations,
             int n, int r, int *aux,
	     int rept=0, int idx=0);
vector<int> getQidsHeaders(vector<string> headers,
                           vector<string> qids);

#endif

