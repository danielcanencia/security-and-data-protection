#ifndef _FREQUENCIES_H
#define _FREQUENCIES_H

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>

using namespace std;

vector<tuple<string, int>>
concatRecordsByIdx(const vector<vector<string>> dataset);
vector<int> calculateFreqs(const vector<vector<string>> dataset);
int findMostDistinctQid(const vector<vector<string>> dataset);
map<string, int> calculateQidFreqs(const vector<vector<string>> dataset,
                                   const int dim);
vector<vector<vector<string>>> createClusters(vector<vector<string>> dataset,
                                              vector<int> qids);

#endif
