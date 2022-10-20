#ifndef _INFO_H
#define _INFO_H

#include <iostream>
#include "../utilities/hierarchy/hierarchy.h"

class Info {
private:
        vector<vector<string>> records;
	vector<vector<string>> attsValues;
        map<int, vector<vector<string>>> hierarchies;
	vector<int> numQids, catQids;
        map<int, int> treeHeights;
        map<int, long double> maxDomSizes;

        void calculateHeights();
        void calculateMaxDomSizes();
public:
        Info(vector<vector<string>> records,
             map<int, vector<vector<string>>> hierarchies,
	     vector<int> numQids, vector<int> catQids);
	vector<int> getNumQids();
	vector<int> getCatQids();
	int valueType(int qid);
	int getSubTreeHeight(string v1, string v2,
			     int attIndex);
	vector<vector<string>> getRecords();
	map<int, vector<vector<string>>> getHierarchies();
	int getTreeHeight(int index);
	long double getMaxDomSize(int index);
	int lowestCommonAncestor(int index);
};


#endif

