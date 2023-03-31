#include "graph.h"

Graph::Graph(){};

Graph::Graph(vector<int> nodesMax, vector<int> qids,
             vector<vector<int>> toPrune, GraphData &graphData) {
  this->graphData = graphData;
  this->qids = qids;
  generateNodesAndEdges(nodesMax, toPrune);
}

void Graph::generateNodesAndEdges(const vector<int> &nodesMax,
                                  vector<vector<int>> toPrune) {
  int nsize = nodesMax.size();
  vector<int> inode(nsize, 0);
  this->graphData.addData(inode);

  int index = 0;
  generateAllNodes(inode, nodesMax, nsize, index);
  // Pruning phase
  this->graphData.pruneNodes(toPrune);
  this->graphData.generateAllEdges();

  return;
}

void Graph::generateAllNodes(const vector<int> &node,
                             const vector<int> &nodesMax, int nsize,
                             int index) {
  vector<vector<int>> aux;

  for (int i = 0; i < nsize; i++) {
    vector<int> nodeAux = node;
    if (node[i] < nodesMax[i]) {
      nodeAux[i] += 1;

      // Create node and edge
      // Generate node and edge (we know the parent node)
      this->graphData.addData(nodeAux);

      index += 1;
      aux.emplace_back(nodeAux);
    }
  }

  for (const vector<int> &entry : aux) {
    generateAllNodes(entry, nodesMax, nsize, index);
  }
}

set<GraphNode> Graph::getRoots() { return this->graphData.getRoots(); }

vector<int> Graph::getQids() const { return this->qids; }

bool Graph::isNodeMarked(GraphNode node) {
  return this->graphData.isNodeMarked(node);
}

int Graph::addGeneralizations(const GraphNode &node, set<GraphNode> &queue) {
  return this->graphData.addGeneralizations(node, queue);
}

void Graph::markGeneralizations(const GraphNode &node) {
  this->graphData.markGeneralizations(node);
}

GraphNode Graph::getFinalKAnon(map<int, map<string, vector<string>>> gens,
                               vector<vector<string>> dataset,
                               vector<int> qids) {
  return graphData.getFinalKAnon(gens, dataset, qids);
}
