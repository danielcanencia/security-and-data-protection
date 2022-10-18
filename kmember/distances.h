#ifndef _DISTANCES_H
#define _DISTANCES_H

#include <iostream>
#include "info.h"


int randomRecord(vector<vector<string>> records);
int furthestRecord(const vector<string> record,
                   int index,
                   const vector<vector<string>> records,
                   const map<int, vector<vector<string>>> hierarchies,
		   const vector<int> numQids,
		   const vector<int> catQids);
long double numDistance(long double v1,
                        long double v2,
			long double maxDomSize);
long double catDistance(const string& v1, const string& v2,
                        Info info, int attIndex);
long double distance(const vector<string>& r1,
                     const vector<string>& r2,
		     Info info);

int find_best_record(vector<vector<string>> records,
		     vector<vector<string>> cluster);
#endif

