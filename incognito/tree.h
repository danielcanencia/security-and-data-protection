#ifndef _TREE_H
#define _TREE_H

#include <iostream>
#include <vector>
#include <string>
#include "treeData.h"

using namespace std;

class Tree {
private:
	vector<vector<int>> nodes;
	TreeData treeData;
	vector<int> qids;
	void generateAllNodesAndEdges(const vector<int>& node,
				   const vector<int>& nodesMax,
				   int nsize, int index);
public:
	Tree(vector<int> nodeMax, vector<int> qids,
	     TreeData& treeData);
	void generateNodesAndEdges(const vector<int>& nodesMax);
	void printNodesTable();
	void printEdgesTable();
};


#endif
