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
#include <iostream> // remove


using namespace std;
namespace fs = std::filesystem;

map<int, vector<vector<string>>> read_directory(
	fs::path const &directory,
	vector<vector<string>>& dataset,
	vector<int>& qids,
  	vector<string>& headersVector,
	int K);
vector<vector<vector<string>>> transposeAndFormat(
                const vector<vector<vector<string>>>& hierarchies);
vector<vector<string>> transpose(const vector<vector<string>>& matrix);
vector<vector<int>> getPermutations(int r, vector<int> nodeMax);
void permute(const vector<int> data,
             vector<vector<int>>& permutations,
             int n, int r, int *aux, int rept=0, int idx=0);

#endif


