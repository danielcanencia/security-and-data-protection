#ifndef _INPUT_H
#define _INPUT_H

#include <vector>
#include <iostream>     // cout, cin
#include <set>
#include <numeric>      // accumulate
#include <algorithm>    // find
#include <tuple>
#include <limits>       // numeric_limits

using namespace std;

const long double readK();
const int readNumberOfQids();
vector<string> readQidNames(const int nqids);
vector<string> readConfidentialAttNames();
vector<double> readWeights(const int nqids, vector<string> qidNames);
tuple<vector<int>, vector<int>> readMetricsQids(vector<int> numQids, vector<int> catQids,
                                                vector<string> qidNames);
bool readParameters(const int datasetSize, const int confAtts,
                    int& K, int& L, long double& P);

#endif
