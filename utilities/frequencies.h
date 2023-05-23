#ifndef _FREQUENCIES_H
#define _FREQUENCIES_H

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <tuple>
#include <set>

using namespace std;

map<string, tuple<int, vector<int>>>
evaluateFrequencyByIdx(vector<vector<string>> dataset, vector<int> qids);
vector<int> calculateFreqs(vector<vector<string>> dataset);
int findMostDistinctQid(const vector<vector<string>> dataset, vector<int> qids);
map<string, int> calculateQidFreqs(const vector<vector<string>> dataset,
                                   const int dim);
vector<vector<vector<string>>> createClusters(vector<vector<string>> dataset,
                                              vector<int> qids);
tuple<vector<map<string, int>>, vector<set<string>>>
createDataMap(vector<vector<string>> dataset, vector<int> confAtts);

#endif
