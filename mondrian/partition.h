#ifndef _PARTITION_H
#define _PARTITION_H

using namespace std;

#include <iostream>
#include <vector>
#include <array>
#include <math.h>
#include <numeric>
#include <algorithm>
#include <string>
#include "../utilities/tree.h"
#include "../utilities/hierarchy.h"
#include "../utilities/frequencies.h"

class Partition {
private:
	vector<int> allowedCuts;
	vector<vector<string>> data;
	vector<int> qids, isQidCat;
	map<int, Tree> trees;
	int K;
	vector<string> generalizations;

	// Methods
	string findMedian(int dimension);
	int normWidth(int dimension);
	vector<int> getAttributeRanges(int dimension);
	vector<Partition> splitPartitionNumeric(int dimension);
	vector<Partition> splitPartitionCategorical(
		int dimension);
public:
	// Constructor
	Partition(vector<vector<string>> data,
              vector<string> generalizations,
              vector<int> qids, vector<int> isQidCat,
		  	  map<int, Tree> trees, int K);
	// Methods
	vector<vector<string>> getResult() const;
	int getNumAllowedCuts();
	void setAllowedCuts(int value, int dim);
	int choose_dimension();
	vector<Partition> splitPartition(int dimension);
	bool isCutAllowed(int index);
};


#endif

