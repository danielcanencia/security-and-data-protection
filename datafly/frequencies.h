#ifndef _FREQUENCIES_H
#define _FREQUENCIES_H


#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <map>
#include "tree.h"

using namespace std;

vector<int> calculateFreqsNR(const vector<vector<string>> dataset);
map<int, int> calculateFreqs(const vector<vector<string>> dataset);
int findMostDistinctQid(const vector<vector<string>> dataset);
void generalizeQid(vector<vector<string>> dataset,
                   int qid, Tree tree);
void supressRecords(vector<vector<string>> dataset, int K,
                    vector<int> frequencies);

#endif

