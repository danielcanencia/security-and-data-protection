#ifndef _GRAPH_H
#define _GRAPH_H

#include "graphData.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Graph {
private:
  vector<vector<int>> nodes;
  GraphData graphData;
  vector<int> qids;
  void generateAllNodes(const vector<int> &node, const vector<int> &nodesMax,
                        int nsize, int index);

public:
  Graph();
  Graph(vector<int> nodeMax, vector<int> qids, vector<vector<int>> toPrune,
        GraphData &treeData);
  void generateNodesAndEdges(const vector<int> &nodesMax,
                             vector<vector<int>> toPrune);
  GraphNode getRoot();
  set<GraphNode> getLeaves();
  vector<int> getQids() const;
  bool isNodeMarked(GraphNode node);
  int addGeneralizations(const GraphNode &node, set<GraphNode> &queue);
  void markGeneralizations(const GraphNode &node);
  GraphNode getFinalKAnon(map<int, map<string, vector<string>>> gens,
                          vector<vector<string>> dataset, vector<int> qids);
};

#endif
