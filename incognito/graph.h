#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include "graphData.h"

using namespace std;

class Graph {
private:
	vector<vector<int>> nodes;
	GraphData graphData;
	vector<int> qids;
	void generateAllNodesAndEdges(const vector<int>& node,
				   const vector<int>& nodesMax,
				   int nsize, int index);
public:
	Graph();
	Graph(vector<int> nodeMax, vector<int> qids,
	      GraphData& treeData);
	void generateNodesAndEdges(const vector<int>& nodesMax);
	set<GraphNode> getRoots();
	vector<int> getQids() const;
	int addGeneralizations(const GraphNode& node, set<GraphNode>& queue);
	void markGeneralizations(const GraphNode& node);
	void printAllKAnon();
	GraphNode getFinalKAnon();
	void printNodesTable();
	void printEdgesTable();
};


#endif
