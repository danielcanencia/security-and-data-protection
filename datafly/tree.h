#ifndef _TREE_H
#define _TREE_H

#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <iostream>

using namespace std;

struct Node {
	bool isLeaf = false;
	int numSubTreeLeaves = 0;
	string parent;
	set<string> children;
	string value;
};

class Tree {
private:
	map<string, Node> nodes;
	vector<string> leaves;
	string addNode(string value, string child,
		       const char* parent = NULL);
	void addLeave(string value, string children);
	Node getCommonAncestor(Node node, string target);
	Node getLowestCommonAncestor(string r1, string r2);

public:
	Tree(vector<vector<string>> hierarchy);
	string getNextGen(string value); 
	int getNumLeaves();
	Node getLowestCommonAncestor(vector<string> values);
	long double getNCP(vector<string> values); //, int nDistinctVals);
	void print();

};

#endif

