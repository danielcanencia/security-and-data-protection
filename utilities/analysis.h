#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <stdio.h>
#include <vector>
#include <iostream>
#include <map>
#include "../utilities/hierarchy.h"
#include "tree.h"
#include<bits/stdc++.h>

using namespace std;


long double calculateNumNCP(const vector<string> atts,
                            long double globalMax,
                            long double globalMin);
long double calculateCatNCP(const int nweights, const double weight,
                            const vector<string> atts, Tree tree);
vector<long double> calculateNCPS(vector<vector<vector<string>>> clusters,
                                  vector<double> weights, vector<int> allQids,
                                  vector<int> numQids, map<int, Tree> trees);
void printAnalysis(vector<vector<vector<string>>> clusters,
                   int numRecords, vector<int> qids,
                   vector<long double> cncps);

#endif

