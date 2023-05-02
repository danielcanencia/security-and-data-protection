#ifndef _GRAPHDATA_H
#define _GRAPHDATA_H

#include "edge.h"
#include "graphNode.h"
#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

class GraphData {
private:
  map<int, GraphNode> nodes;
  vector<Edge> edges;
  GraphNode root;
  int idCount = 0;

  void printAllKAnon(const GraphNode &node, vector<int> kList);
  void getAllKAnon(const GraphNode &node, vector<int> kList,
                   set<GraphNode> &nodes);

public:
  GraphData();
  void addData(vector<int> data);
  int contains(const vector<int> &node);
  void pruneNodes(vector<vector<int>> toPrune);
  void generateAllEdges();
  void setRoot();
  GraphNode getRoot();
  set<GraphNode> getLeaves();
  bool isNodeMarked(GraphNode node);
  int addGeneralizations(const GraphNode &node, set<GraphNode> &queue);
  void markGeneralizations(const GraphNode &node);
  vector<GraphNode> getChildren(GraphNode node);
  GraphNode getFinalKAnon(map<int, map<string, vector<string>>> gens,
                          vector<vector<string>> dataset, vector<int> qids);
};

#endif
