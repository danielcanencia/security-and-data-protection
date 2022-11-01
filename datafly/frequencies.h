#ifndef _FREQUENCIES_H
#define _FREQUENCIES_H


#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <tuple>
#include "tree.h"
#include "../utilities/hierarchy.h"

using namespace std;


vector<int> calculateFreqs(const vector<vector<string>> dataset);
vector<tuple<vector<int>, int>> calculateFreqsByIdx(
	const vector<vector<string>> dataset);
int findMostDistinctQid(const vector<vector<string>> dataset);
bool isKAnonSatisfied(const vector<vector<string>> dataset,
                      const int K);
void generalizeQid(vector<vector<string>> dataset,
                   int qid, Tree tree);
void supressRecords(vector<vector<string>> dataset, int K,
                    vector<int> frequencies);

#endif

