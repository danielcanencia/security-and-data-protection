#ifndef _TREE_H
#define _TREE_H

#include <iostream>
#include <vector>
#include <string>


using namespace std;

class Tree {
private:
	//vector<Edge> edges;
	vector<string> edges;
	vector<vector<string>> nodes;
public:
	Tree(vector<vector<vector<string>>> hierarchies);
	void constructNodesAndEdges(vector<vector<vector<string>>> hierarchies);
};


#endif
