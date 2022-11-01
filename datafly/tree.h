#ifndef _TREE_H
#define _TREE_H

#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct Node {
	int parent = -1;
	vector<int> children;
	string value;
};

class Tree {
private:
	//const int qid;
	vector<Node> nodes;
	vector<Node> leaves;
	int addNode(string value, bool childIsLeaf,
		    int child, const char* parent = NULL);
	int addLeave(string value, string children);
	/*int getGenLevel(string value);
	int getMaxGenLevel(vector<string> values);*/
public:
	Tree(vector<vector<string>> hierarchy);
	string getNextGen(string value); 
	vector<string> getNextGens(vector<string> values);
};

#endif

