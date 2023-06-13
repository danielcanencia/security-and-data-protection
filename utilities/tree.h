/*! \file tree.h
    \brief Fichero de cabecera del archivo tree.cpp.
*/

#ifndef _TREE_H
#define _TREE_H

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

/*! Clase Nodo. Su función es simular el nodo de un árbol jerárquico.
*/
struct Node {
  bool isLeaf = false;
  int numSubTreeLeaves = 0;
  string parent;
  set<string> children;
  string value;
  int depth;
};

/*! Clase Tree. Su función es simular un árbol jerárquico correspondiente
    a un atributo específico.
*/
class Tree {
private:
  map<string, Node> nodes;
  vector<string> leaves;
  int height;
  string addNode(string value, string child, const char *parent = NULL);
  void addLeave(string value, string children, int depth);
  int calculateHeight();
  Node getCommonAncestor(Node node, string target);
  Node getLowestCommonAncestor(string r1, string r2);

public:
  string root;
  // Constructors
  Tree();
  Tree(vector<vector<string>> hierarchy);
  // Methods
  int getHeight();
  int getHeight(string value);
  int getHeight(string value, int &depth);
  int getNumSubTreeLeaves(string value);
  bool isChild(string node, string child);
  vector<string> getDirectChildren(string value);
  vector<string> getAllChildren(string value);
  vector<string> getChildrenInLevel(string value);
  Node *getDescendant(Node *node, string target);
  int getParentDiff(string value1, string value2);
  int getDepth(string value);
  string getNextGen(string value);
  int getNumLeaves();
  Node getLowestCommonAncestor(vector<string> values);
  long double getNCP(vector<string> values);
};

#endif
