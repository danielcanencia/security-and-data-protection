#ifndef _GRAPHDATA_H
#define _GRAPHDATA_H

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include "edge.h"
#include "graphNode.h"

using namespace std;


class GraphData {
private:
	vector<GraphNode> nodes;
	vector<Edge> edges;
	int idCount = 0;

	void printAllKAnon(const GraphNode& node,
		   vector<int> kList);
	void getAllKAnon(const GraphNode& node, vector<int> kList,
		   set<GraphNode>& nodes);
public:
	GraphData();
	void addData(vector<int> data);
	int contains(const vector<int>& node);
	void generateAllEdges();
	set<GraphNode> getRoots();
	int addGeneralizations(const GraphNode& node,
			set<GraphNode>& queue);
	void markGeneralizations(const GraphNode& node);
	vector<GraphNode> getChildren(GraphNode node);
	void printAllKAnon();
	GraphNode getFinalKAnon(map<int, map<string, vector<string>>> gens,
							vector<vector<string>> dataset,
							vector<int> qids);
	void printNodesTable();
	void printEdgesTable();
};

#endif

