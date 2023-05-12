#ifndef _ANONYMITY_H
#define _ANONYMITY_H

#include "../utilities/hierarchy.h"
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

bool isSplitKAnonymous(vector<vector<string>> split, const int K);
bool isSplitLDiverse(vector<vector<string>> split, vector<int> confAtts,
                     const int L);
bool isSplitTClose(vector<vector<string>> split, vector<vector<string>> data,
                   tuple<vector<map<string, int>>, vector<set<string>>> dataMap,
                   vector<int> confAtts, const long double T);
bool isSplitValid(vector<vector<vector<string>>> splits,
                  tuple<vector<map<string, int>>, vector<set<string>>> dataMap,
                  vector<vector<string>> dataset, vector<int> confAtts,
                  const int K, const int L, const long double T);

#endif
