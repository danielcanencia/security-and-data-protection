#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <stdio.h>
#include <vector>
#include <iostream>
#include <map>
#include<bits/stdc++.h>

using namespace std;

void printAnalysis(vector<vector<vector<string>>> clusters,
                   int numRecords, vector<int> qids,
                   vector<long double> cncps);

#endif

