#ifndef _TREE_H
#define _TREE_H

#include <string>
#include <string.h>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

struct Node {
	int parent = -1;
	set<int> children;
	int childrenAreLeaves = -1;
	string value;
};

class Tree {
private:
	vector<Node> nodes;
	vector<Node> leaves;
	/*int getGenLevel(string value);
	int getMaxGenLevel(vector<string> values);*/
public:
	Tree(vector<vector<string>> hierarchy);
	int addNode(string value, bool childIsLeaf,
		    int child, const char* parent = NULL);
	int addLeave(string value, string children);
	string getNextGen(string value); 
	vector<string> getNextGens(vector<string> values);
	void print();

};

#endif

