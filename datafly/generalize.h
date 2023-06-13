/*! \file generalize.h
    \brief Fichero de cabecera del archivo generalize.cpp.
*/

#ifndef _GENERALIZE_H
#define _GENERALIZE_H

#include "../utilities/frequencies.h"
#include "../utilities/hierarchy.h"
#include "../utilities/tree.h"
#include <vector>

using namespace std;

bool isKAnonSatisfied(const vector<vector<string>> dataset, const int K);
void generalizeQid(vector<vector<string>> &dataset, const int qid, Tree tree);
void supressRecords(vector<vector<string>> &dataset, vector<int> qids,
                    const int K);

#endif
