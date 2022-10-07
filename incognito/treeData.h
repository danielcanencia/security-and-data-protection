#ifndef _TREEDATA_H
#define _TREEDATA_H

#include <iostream>
#include <vector>
#include <tuple>
#include "edge.h"
#include "node.h"

using namespace std;


class TreeData {
private:
	vector<Node> nodes;
	vector<Edge> edges;
	int idCount = 0;
public:
	TreeData();
	void addData(vector<int> data);
	int contains(const vector<int>& node);
	int contains(int from, int to);
	void generateAllEdges();
	void printNodesTable();
	void printEdgesTable();
};

#endif

