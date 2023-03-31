#ifndef _PARTITION_H
#define _PARTITION_H

using namespace std;

#include "../utilities/frequencies.h"
#include "../utilities/hierarchy.h"
#include "../utilities/tree.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <math.h>
#include <numeric>
#include <string>
#include <vector>

class Partition {
private:
  vector<int> allowedCuts;
  vector<vector<string>> data;
  vector<int> qids, isQidCat;
  map<int, Tree> trees;
  vector<int> confAtts;
  int K, L;
  long double T;
  vector<string> generalizations;

  // Methods
  string findMedian(int dimension);
  int normWidth(int dimension);
  vector<int> getAttributeRanges(int dimension);
  bool isSplitKAnonymous(vector<vector<string>> split);
  bool isSplitLDiverse(vector<vector<string>> split);
  bool isSplitTClose(vector<vector<string>> split);
  bool isSplitValid(vector<vector<string>> split);
  vector<Partition> splitPartitionNumeric(int dimension);
  vector<Partition> splitPartitionCategorical(int dimension);

public:
  // Constructor
  Partition(vector<vector<string>> data, vector<string> generalizations,
            vector<int> qids, vector<int> isQidCat, map<int, Tree> trees,
            vector<int> confAtts, int K, int L, long double T);
  // Methods
  vector<vector<string>> getResult() const;
  int getNumAllowedCuts();
  void setAllowedCuts(int value, int dim);
  int chooseDimension();
  vector<Partition> splitPartition(int dimension);
  bool isCutAllowed(int index);
};

#endif
