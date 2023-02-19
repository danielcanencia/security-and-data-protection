#ifndef _EVALUATE_H
#define _EVALUATE_H

using namespace std;

#include <iostream>
#include <vector>
#include <tuple>
#include "graph.h"
#include "../utilities/frequencies.h"
#include "../utilities/hierarchy.h"

tuple<vector<vector<string>>, vector<vector<vector<string>>>> incognito(
								vector<vector<string>> dataset,
								map<int, vector<vector<string>>> hierarchies,
								vector<int> qids, vector<int> confAtts,
                                const int K, const int L, const long double P);

#endif
