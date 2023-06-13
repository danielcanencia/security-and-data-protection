/*! \file evaluate.h
    \brief Fichero de cabecera del archivo evaluate.cpp.
*/

#ifndef _EVALUATE_H
#define _EVALUATE_H

using namespace std;

#include "partition.h"
#include <iostream>
#include <vector>

vector<vector<vector<string>>> evaluate(Partition partition);
void evaluate(Partition partition, vector<Partition> &result);
vector<vector<vector<string>>>
mondrian(vector<vector<string>> dataset,
         map<int, vector<vector<string>>> hierarchies, vector<int> allQids,
         vector<int> isQidCat, vector<int> confAtts, const int K, const int L,
         const long double T);

#endif
