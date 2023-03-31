#include "tree.h"
#include <iostream>

Tree::Tree(){};

Tree::Tree(vector<vector<string>> hierarchy) {

  for (const auto &entries : hierarchy) {
    addLeave(entries[0], entries[1], entries.size() - 1);

    size_t i;
    string child = entries[0];
    for (i = 1; i < entries.size() - 1; i++) {
      child = addNode(entries[i], child, entries[i + 1].c_str());
    }

    addNode(entries[i], child);
  }

  // Calculate tree height
  this->height = calculateHeight();
}

void Tree::addLeave(string value, string parent, int depth) {
  // Check if leave is already present
  if (this->nodes.count(value))
    return;

  Node node;
  node.value = value;
  node.parent = parent;
  node.isLeaf = true;
  node.depth = depth;
  this->nodes[value] = node;

  this->leaves.emplace_back(value);
}

string Tree::addNode(string value, string child, const char *parent) {

  // Node already present
  if (this->nodes.count(value)) {
    this->nodes[value].children.insert(child);
    this->nodes[value].numSubTreeLeaves += 1;
    return value;
  }

  // Node not present
  Node node;
  node.value = value;
  if (parent)
    node.parent = (string)parent;
  else {
    node.parent = value;
    this->root = value;
  }

  node.depth = this->nodes[child].depth - 1;
  node.children = {child};
  node.numSubTreeLeaves += 1;
  this->nodes[value] = node;

  return value;
}

int Tree::calculateHeight() {
  set<int> depths;

  for (const string &value : leaves)
    depths.insert(this->nodes[value].depth);

  return *max_element(depths.begin(), depths.end()) + 1;
}

int Tree::getHeight() { return this->height; }

int Tree::getHeight(string value) {
  const Node node = this->nodes[value];
  int maxDepth = node.depth;

  for (const auto &child : node.children)
    getHeight(child, maxDepth);

  return maxDepth - node.depth + 1;
}

int Tree::getHeight(string value, int &depth) {
  const Node node = this->nodes[value];
  int aux;

  for (const auto &child : node.children) {
    aux = getDepth(child);
    if (aux > depth)
      depth = aux;

    getHeight(child, depth);
  }

  return depth;
}

int Tree::getNumSubTreeLeaves(string value) {
  return this->nodes[value].numSubTreeLeaves;
}

int Tree::getDepth(string value) { return this->nodes[value].depth; }

bool Tree::isChild(string node, string target) {
  set<string> children = this->nodes[node].children;

  if (find(children.begin(), children.end(), target) != children.end())
    return true;

  for (const auto &child : children) {
    if (isChild(child, target))
      return true;
  }

  return false;
}

vector<string> Tree::getDirectChildren(string value) {
  set<string> children = this->nodes[value].children;
  return vector<string>(children.begin(), children.end());
}

vector<string> Tree::getAllChildren(string value) {
  vector<string> children;

  for (const auto &child : this->nodes[value].children) {
    children.emplace_back(child);
    vector<string> next = getAllChildren(child);
    children.insert(children.begin(), next.begin(), next.end());
  }

  return children;
}

vector<string> Tree::getChildrenInLevel(string value) {
  vector<string> children;
  const int level = this->nodes[value].depth + 1;

  for (auto const &[k, v] : this->nodes) {
    if (v.depth == level)
      children.emplace_back(k);
  }

  return children;
}

string Tree::getNextGen(string value) {

  // Node present
  if (this->nodes.count(value)) {
    string parent = this->nodes[value].parent;
    if (parent == value)
      return value;

    return parent;
  }

  // Not found
  throw "Error: Element not found in the tree";
}

int Tree::getNumLeaves() { return this->leaves.size(); }

Node Tree::getCommonAncestor(Node node, string target) {
  if (node.value == target)
    return this->nodes[node.value];

  // It is a direct child
  if (find(node.children.begin(), node.children.end(), target) !=
      node.children.end())
    return node;

  Node n = this->nodes[node.parent];
  // vector<string> children(node.children.begin(),
  //			node.children.end());
  vector<string> children = getAllChildren(node.value);

  /*cout << "node: ";
  cout << node.value << endl;
  cout << "Parent: ";
  cout << n.value << endl;
  cout << "Children: ";
  for (const auto& a : children)
          cout << a + ", ";
  cout << endl;

  cout << "Depths: ";
  cout << getDepth(node.value);
  cout << getDepth(target) << endl;*/

  if (find(children.begin(), children.end(), target) == children.end())
    return getCommonAncestor(n, target);

  // if (getDepth(node.value) >= getDepth(target))
  //	return getCommonAncestor(n, target);

  /*while (find(children.begin(), children.end(), target)
          == children.end()) {

          n = this->nodes[n.parent];
          children.insert(children.begin(),
                  n.children.begin(), n.children.end());
  }*/

  return n;
}

Node Tree::getLowestCommonAncestor(string r1, string r2) {

  Node n1 = this->nodes[r1];
  Node n2 = this->nodes[r2];
  Node node;
  string target;

  if (r1 == r2)
    return this->nodes[r1];

  if (this->nodes[n1.parent].numSubTreeLeaves <
      this->nodes[n2.parent].numSubTreeLeaves) {
    node = this->nodes[n1.parent];
    target = n2.value;
  } else {
    node = this->nodes[n2.parent];
    target = n1.value;
  }

  return getCommonAncestor(node, target);
}

Node Tree::getLowestCommonAncestor(vector<string> values) {
  Node node;
  string aux = values[0];

  if (values.size() == 1) {
    return this->nodes[aux];
  }

  for (size_t i = 1; i < values.size(); i++) {
    node = getLowestCommonAncestor(aux, values[i]);
    // Return root node if we've reached it
    if (node.parent == node.value)
      return node;
    aux = node.value;
  }

  return node;
}

long double Tree::getNCP(vector<string> values) {
  // Get unique values only
  vector<string> aux = values;
  vector<string>::iterator it = unique(aux.begin(), aux.end());
  aux.resize(distance(aux.begin(), it));

  int subTreeLeaves = getLowestCommonAncestor(aux).numSubTreeLeaves;
  long double ncp = (long double)subTreeLeaves;
  return ncp;
}
