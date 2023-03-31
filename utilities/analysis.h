#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include "hierarchy.h"
#include "tree.h"
#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <stdio.h>
#include <vector>

using namespace std;

long double calculateNumNCP(const vector<string> atts, long double globalMax,
                            long double globalMin);
long double calculateCatNCP(const vector<string> atts, Tree tree);
vector<long double> calculateNCPS(vector<vector<vector<string>>> clusters,
                                  vector<double> weights, vector<int> allQids,
                                  vector<int> numQids, map<int, Tree> trees);
void calculateGCP(vector<vector<vector<string>>> clusters, int numRecords,
                  vector<int> qids, vector<long double> cncps);

#endif
