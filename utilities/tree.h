#ifndef _TREE_H
#define _TREE_H

#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>

using namespace std;

struct Node {
	bool isLeaf = false;
	int numSubTreeLeaves = 0;
	string parent;
	set<string> children;
	string value;
	int depth;
};

class Tree {
private:
	map<string, Node> nodes;
	vector<string> leaves;
	string addNode(string value, string child,
		       const char* parent = NULL);
	void addLeave(string value, string children, int depth);
	Node getCommonAncestor(Node node, string target);
	Node getLowestCommonAncestor(string r1, string r2);

public:
	string root;
	// Constructors
	Tree();
	Tree(vector<vector<string>> hierarchy);
	// Methods
	int getNumSubTreeLeaves(string value);
	bool isChild(string node, string child);
	vector<string> getDirectChildren(string value);
	vector<string> getAllChildren(string value);
	vector<string> getChildrenInLevel(string value);
	Node* getDescendant(Node* node, string target);
	int getParentDiff(string value1, string value2);
	int getDepth(string value);
	string getNextGen(string value); 
	int getNumLeaves();
	Node getLowestCommonAncestor(vector<string> values);
	long double getNCP(vector<string> values);
};

#endif

