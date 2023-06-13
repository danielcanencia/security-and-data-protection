#include "graphData.h"

GraphData::GraphData() {}

void GraphData::addData(vector<int> data) {
  int nodeId = this->contains(data);
  if (nodeId == -1) {
    this->nodes[this->idCount] = GraphNode(this->idCount, data);
    this->idCount++;
  }
}

int GraphData::contains(const vector<int> &node) {
  for (const auto &[id, entry] : this->nodes) {
    if (entry.isEqual(node))
      return entry.getId();
  }
  return -1;
}

void GraphData::pruneNodes(vector<vector<int>> toPrune) {
  while (!toPrune.empty()) {
    vector<int> node1 = toPrune[0];
    toPrune.erase(toPrune.begin());

    // Find node
    bool flag;
    for (const auto &[id, node2] : this->nodes) {
      vector<int> data = node2.getData();
      flag = true;
      for (size_t w = 0; w < node1.size(); w++) {
        if (node1[w] != data[w]) {
          flag = false;
          break;
        }
      }
      // Prune node
      if (flag) {
        this->nodes.erase(node2.getId());
        break;
      }
    }
  }
}

void GraphData::generateAllEdges() {
  for (const auto &[from, node1] : this->nodes) {
    GraphNode node = this->nodes[from];
    vector<int> to;

    for (const auto &[id, node2] : this->nodes) {
      if (from == id)
        continue;
      if (node.isChild(node2))
        to.emplace_back(id);
    }
    for (const auto &entry : to)
      this->edges.emplace_back(Edge(from, entry));
  }
}

void GraphData::setRoot() {
  bool flag;

  for (const auto &entry1 : this->edges) {
    int childId = entry1.getChild();
    flag = true;
    for (const auto &entry2 : this->edges) {
      if (entry2.getParent() == childId) {
        flag = false;
        break;
      }
    }

    if (flag) {
      this->root = this->nodes[childId];
      return;
    }
  }

  this->root = GraphNode();
}

GraphNode GraphData::getRoot() { return this->root; }

set<GraphNode> GraphData::getLeaves() {
  set<GraphNode> roots;
  map<int, int> rootsIds;
  bool flag;

  // Get root node's ids
  for (const auto &entry1 : this->edges) {
    int parentId = entry1.getParent();
    flag = true;
    for (const auto &entry2 : this->edges) {
      if (entry2.getChild() == parentId) {
        flag = false;
        break;
      }
    }
    if (flag) {
      roots.insert(this->nodes[entry1.getParent()]);
    }
  }

  return roots;
}

bool GraphData::isNodeMarked(GraphNode node) {
  return this->nodes[node.getId()].marked();
}

int GraphData::addGeneralizations(const GraphNode &node,
                                  set<GraphNode> &queue) {
  vector<GraphNode> children = getChildren(node);
  if (children.empty())
    return -1;

  for (const GraphNode &child : children) {
    if (!child.marked())
      queue.insert(child);
  }

  return 0;
}

void GraphData::markGeneralizations(const GraphNode &node) {
  this->nodes[node.getId()].setKAnon();
  this->nodes[node.getId()].mark();

  for (const GraphNode &child : getChildren(node)) {
    this->nodes[child.getId()].mark();
    this->nodes[child.getId()].setKAnon();
  }
}

vector<GraphNode> GraphData::getChildren(GraphNode node) {
  vector<GraphNode> children;

  for (const Edge &edge : this->edges) {
    if (edge.getParent() == node.getId()) {
      children.emplace_back(this->nodes[edge.getChild()]);
    }
  }
  return children;
}

void GraphData::getAllKAnon(const GraphNode &node, vector<int> kList,
                            set<GraphNode> &nodes) {
  for (const GraphNode &node : getChildren(node)) {
    if (find(kList.begin(), kList.end(), node.getId()) != kList.end()) {
      nodes.insert(node);
      kList.emplace_back(node.getId());
      getAllKAnon(node, kList, nodes);
    }
  }
}

GraphNode GraphData::getFinalKAnon(map<int, map<string, vector<string>>> gens,
                                   vector<vector<string>> dataset,
                                   vector<int> qids) {
  set<GraphNode> res;
  for (const auto &[id, node] : this->nodes) {
    if (node.isKAnon()) {
      vector<int> kList;
      res.insert(node);
      getAllKAnon(node, kList, res);
    }
  }

  // If no node matches the criteria return root node
  if (res.size() == 0)
    return getRoot();

  // Get one node based on criteria. One that maximizes data
  // utility metric (the one that presents the maximum number of clusters)
  int max = -1;
  GraphNode finalNode;
  for (const GraphNode &node : res) {
    vector<int> freqs = node.evaluateFrequency(gens, dataset, qids);

    if ((int)freqs.size() > max || max == -1) {
      max = freqs.size();
      finalNode = node;
    }
  }

  return finalNode;
}
