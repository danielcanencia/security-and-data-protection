#ifndef _ANONYMITY_H
#define _ANONYMITY_H

#include <vector>
#include <set>
#include <string>
#include <map>
#include "../utilities/hierarchy.h"

using namespace std;

/*bool isSplitKAnonymous(vector<map<string, int>> qidFreqs,
					   const int K);
bool isSplitLDiverse(vector<map<string, int>> confFreqs,
					 const long double L);
bool isSplitTClose(vector<map<string, int>> confFreqs,
				   int splitSize,
				   vector<string> confKeys,
				   vector<vector<string>> data,
                   int confAtts, const long double P);
bool isSplitValid(vector<map<string, int>> qidFreqs,
				  vector<map<string, int>> confFreqs,
				  vector<vector<string>> dataset,
				  vector<string> confKeys,
				  const int K, const int L, const long double P);*/
bool isSplitKAnonymous(vector<vector<string>> split,
					   const int K);
bool isSplitLDiverse(vector<vector<string>> split,
                     vector<int> confAtts, const long double L);
bool isSplitTClose(vector<vector<string>> split, vector<vector<string>> data,
                   vector<int> confAtts, const long double P);
bool isSplitValid(vector<vector<vector<string>>> splits,
				  vector<vector<string>> dataset, vector<int> confAtts,
				  const int K, const int L, const long double P);

#endif
