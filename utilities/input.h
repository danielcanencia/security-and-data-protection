#ifndef _INPUT_H
#define _INPUT_H

#include <vector>
#include <iostream>     // cout, cin
#include <set>
#include <numeric>      // accumulate
#include <algorithm>    // find
#include <tuple>

using namespace std;

const int readK();
const int readNumberOfQids();
vector<string> readQidNames(const int nqids);
vector<double> readWeights(const int nqids, vector<string> qidNames);
tuple<vector<int>, vector<int>> readMetricsQids(vector<int> numQids, vector<int> catQids,
                                                vector<string> qidNames);


#endif
