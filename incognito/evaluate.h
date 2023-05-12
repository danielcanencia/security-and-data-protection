#ifndef _EVALUATE_H
#define _EVALUATE_H

using namespace std;

#include "../utilities/frequencies.h"
#include "../utilities/hierarchy.h"
#include "graph.h"
#include <iostream>
#include <tuple>
#include <vector>

tuple<vector<vector<string>>, vector<vector<vector<string>>>>
incognito(vector<vector<string>> dataset,
          map<int, vector<vector<string>>> hierarchies, vector<int> qids,
          vector<int> confAtts, const int K, const int L, const long double T);

#endif
