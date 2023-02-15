#ifndef _EVALUATE_H
#define _EVALUATE_H

using namespace std;

#include <iostream>
#include <vector>
#include "partition.h"

vector<vector<string>> evaluate(Partition partition);
void evaluate(Partition partition,
	      vector<Partition>& result);
vector<vector<string>> mondrian (vector<vector<string>> dataset,
								 map<int, vector<vector<string>>> hierarchies,
								 vector<int> allQids, vector<int> isQidCat,
								 const int K);

#endif

