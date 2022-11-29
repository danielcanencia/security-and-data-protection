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
	int K;
	vector<string> generalizations;

	int choose_dimension();
	int normWidth(int dimension);
	vector<int> find_median(int dimension);
	bool isCutAllowed(int index);
	/*vector<Partition> cut(vector<int> split,
		int dimension, Tree tree);*/
public:
	Partition(vector<vector<string>> data,
                  vector<string> generalizations,
                  vector<int> qids, int K);
	//vector<vector<string>> evaluate(vector<Tree> Trees);
	vector<Partition> evaluate(vector<Tree> Trees);
	void evaluate(vector<Tree> Trees,
               	      vector<Partition> result);
};


#endif

