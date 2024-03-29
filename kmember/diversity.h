/*! \file diversity.h
    \brief Fichero de cabecera del archivo diversity.cpp.
*/

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
bool isSensitive(string majorityClass, vector<string> values);
string readConfidentialAttName();
int readDiversityPenalty();
int readDiversity();
vector<string> readSensitiveValues(vector<string> values);

#endif
