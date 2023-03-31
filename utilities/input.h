#ifndef _INPUT_H
#define _INPUT_H

#include <algorithm> // find
#include <iostream>  // cout, cin
#include <limits>    // numeric_limits
#include <numeric>   // accumulate
#include <set>
#include <tuple>
#include <vector>

using namespace std;

const long double readK();
const int readNumberOfQids();
vector<string> readQidNames(const int nqids);
vector<string> readConfidentialAttNames();
vector<double> readWeights(const int nqids, vector<string> qidNames);
tuple<vector<int>, vector<int>> readMetricsQids(vector<int> numQids,
                                                vector<int> catQids,
                                                vector<string> headers);
long double readParameter(const string privacyDef, const string parameter,
                          const int datasetSize);
bool readParameters(const int datasetSize, const int confAtts, int &K, int &L,
                    long double &T);

#endif
