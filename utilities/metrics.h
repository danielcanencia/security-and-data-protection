#ifndef _METRICS_H
#define _METRICS_H

#include "tree.h"
#include <bits/stdc++.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

void calculateDM(vector<vector<vector<string>>> clusters, const int tableSize,
                 const int K, const int L, const long double P);
void calculateCAVG(vector<vector<vector<string>>> clusters, const int tableSize,
                   const int K, const int L, const long double P);
void calculateGenILoss(vector<vector<string>> transposedDataset,
                       map<int, Tree> trees, const vector<int> allQids,
                       const vector<int> catQids, const vector<int> numQids,
                       const int tableSize);
#endif
