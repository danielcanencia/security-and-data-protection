#ifndef _PARTITION_H
#define _PARTITION_H

using namespace std;

#include <iostream>
#include <vector>
#include <array>
#include <math.h>
#include <numeric>
#include "tree.h"
#include "../utilities/hierarchy.h"
#include "../utilities/frequencies.h"

class Partition {
private:
	vector<int> allowedCuts;
	vector<vector<string>> data;
	vector<int> qids;
	vector<Tree> trees;
	int K;
	vector<string> generalizations;
	vector<int> numSubTreeLeaves;

	int getParentDiff(const string x,
		const string y, int dimension);
	/*vector<Partition> cut(vector<int> split,
		int dimension, Tree tree);*/
public:
	Partition(vector<vector<string>> data,
                  vector<string> generalizations,
                  vector<int> numSubTreeLeaves,
                  vector<int> qids, vector<Tree> trees,
                  int K);
	void printData() const;
	vector<vector<string>> getResult() const;
	int getNumAllowedCuts();
	void setAllowedCuts(int value, int dim);
	int choose_dimension();
	int normWidth(int dimension);
	int find_median(int dimension);
	bool isCutAllowed(int index);
	vector<Partition> splitPartitionCategorical(
		int dimension);
	//vector<vector<string>> evaluate(vector<Tree> Trees);
	//vector<Partition> evaluate();
	//void evaluate(vector<Partition> result);
};


#endif

