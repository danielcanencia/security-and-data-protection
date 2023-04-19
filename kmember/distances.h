#ifndef _DISTANCES_H
#define _DISTANCES_H

#include "../utilities/tree.h"
#include "diversity.h"
#include "info.h"
#include <iostream>
#include <numeric>
#include <vector>

int randomRecord(vector<vector<string>> records);
int furthestRecord(vector<string> record, int index,
                   vector<vector<string>> records,
                   map<int, vector<vector<string>>> hierarchies,
                   map<int, Tree> trees, vector<int> numQids,
                   vector<int> catQids);
long double numDistance(long double v1, long double v2, long double maxDomSize);
long double catDistance(string &v1, string &v2, Info info, int attIndex);
long double distance(vector<string> &r1, vector<string> &r2, Info info);
int findBestRecord(vector<vector<string>> records,
                   vector<vector<string>> cluster,
                   map<int, vector<vector<string>>> hierarchies,
                   map<int, Tree> trees, vector<int> numQids,
                   vector<int> catQids, int confAtt, const int L,
                   vector<string> sensitiveValues, const int diversityPenalty,
                   const int diversity);
int findBestCluster(map<int, vector<vector<string>>> clusters,
                    vector<string> record,
                    map<int, vector<vector<string>>> hierarchies,
                    map<int, Tree> trees, vector<int> numQids,
                    vector<int> catQids);
#endif
