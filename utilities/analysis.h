#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <stdio.h>
#include <vector>
#include <iostream>
#include <map>
#include<bits/stdc++.h>

using namespace std;

void printAnalysis(vector<vector<vector<string>>> clusters,
		   int numRecords,
                   map<int, vector<vector<string>>> hierarchies,
                   vector<int> qids, vector<double> weights,
                   vector<long double> cncps);

#endif

