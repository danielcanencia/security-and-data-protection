#ifndef _ANONYMITY_H
#define _ANONYMITY_H

#include "../utilities/frequencies.h"
#include "../utilities/hierarchy.h"
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

bool isSplitKAnonymous(vector<vector<string>> split, vector<int> qids,
                       const int K);
bool isSplitLDiverse(vector<vector<string>> split, vector<int> confAtts,
                     const int L);
bool isSplitTClose(vector<vector<string>> split, vector<vector<string>> data,
                   tuple<vector<map<string, int>>, vector<set<string>>> dataMap,
                   vector<int> confAtts, const long double T);
bool isSplitValid(vector<vector<string>> split,
                  tuple<vector<map<string, int>>, vector<set<string>>> dataMap,
                  vector<vector<string>> dataset, vector<int> qids,
                  vector<int> confAtts, const int K, const int L,
                  const long double T);
#endif
