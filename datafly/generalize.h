#ifndef _GENERALIZE_H
#define _GENERALIZE_H

#include <vector>
#include "../utilities/hierarchy.h"
#include "tree.h"
#include "frequencies.h"

using namespace std;


bool isKAnonSatisfied(const vector<vector<string>> dataset,
                      const int K);
void generalizeQid(vector<vector<string>> &dataset,
                   const int qid, Tree tree);
void supressRecords(vector<vector<string>> &dataset, const int K);


#endif

