#ifndef _FREQUENCIES_H
#define _FREQUENCIES_H


#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

vector<tuple<string, int>> concatRecordsByIdx(
        const vector<vector<string>> dataset);
vector<int> calculateFreqs(const vector<vector<string>> dataset);
int findMostDistinctQid(const vector<vector<string>> dataset);

#endif

