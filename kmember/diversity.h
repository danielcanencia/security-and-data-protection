#ifndef _DIVERSITY_H
#define _DIVERSITY_H

#include "../utilities/hierarchy.h"
#include <algorithm> // max_element
#include <iostream>  // cout, cin
#include <limits>    // numeric_limits
#include <map>
#include <string>
#include <vector>

using namespace std;

bool isDiverse(vector<vector<string>> cluster, const int confAtt, const int L);
string majorityClass(vector<vector<string>> cluster, const int confAtt);
bool isSensitive(string majorityClass, const int confAtt,
                 vector<vector<string>> cluster);
string readConfidentialAttName();
int readDiversityPenalty();
int readDiversity();
vector<string> readSensitiveValues(vector<string> values);

#endif
