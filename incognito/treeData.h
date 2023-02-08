#ifndef _TREEDATA_H
#define _TREEDATA_H

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include "edge.h"
#include "node.h"

using namespace std;


class TreeData {
private:
	vector<Node> nodes;
	vector<Edge> edges;
	int idCount = 0;

	void printAllKAnon(const Node& node,
		   vector<int> kList);
	void getAllKAnon(const Node& node, vector<int> kList,
		   vector<Node>& nodes);
public:
	TreeData();
	void addData(vector<int> data);
	int contains(const vector<int>& node);
	void generateAllEdges();
	vector<Node> getRoots();
	int addGeneralizations(const Node& node,
			       vector<Node>& queue);
	void markGeneralizations(const Node& node);
	vector<Node> getChildren(Node node);
	void printAllKAnon();
	Node getFinalKAnon();
	void printNodesTable();
	void printEdgesTable();
};

#endif

