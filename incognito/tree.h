#ifndef _TREE_H
#define _TREE_H

#include <iostream>
#include <vector>
#include <string>


using namespace std;

class Tree {
private:
	vector<vector<int>> nodes;
	//vector<Edge> edges;
	void generateAllNodesAndEdges(vector<int> node,
				   vector<int> nodesMax,
				   int nsize, int index);
	
public:
	Tree(vector<int> nodeMax);
	void generateNodesAndEdges(vector<int> nodesMax);
};


#endif
