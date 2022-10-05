#include "tree.h"

Tree::Tree(vector<int> nodesMax) {
	generateNodesAndEdges(nodesMax);
}

void Tree::generateNodesAndEdges(vector<int> nodesMax) {
	int nsize = nodesMax.size();
	vector<int> inode(nsize, 0);
	
	// Create first node and edge
	this->nodes.emplace_back(inode);
	//this->edges = new Edge(1, 2);
	for (const int& entry : inode) {
		cout << to_string(entry) + ", ";
	}
	cout << endl;


	// Copy nodesMax
	//vector<int> node;

	int index = 2;
	generateAllNodesAndEdges(inode, nodesMax,
 				 nsize, index);
	cout << "ddddddddddddddddddddddddddd" << endl;
	return ;
}

void Tree::generateAllNodesAndEdges(vector<int> node,
	   		      vector<int> nodesMax,
			      int nsize, int index) {
	int to = 0;
	vector<vector<int>> aux;

	for (int i=0; i < nsize; i++) {
		vector<int> nodeAux = node;
		if (node[i] + 1 <= nodesMax[i]) {
			nodeAux[i] += 1;

			// Create node and edge
			this->nodes.emplace_back(nodeAux);
			to += 1;
			//this->edges = new Edge(index, to);
			for (const int& entry : nodeAux)
				cout << to_string(entry) + ", ";
			cout << endl;

			index += 1;
			// Generate parent nodes
			/*generateAllNodesAndEdges(node, nodesMax,
					      nsize, index);*/
			aux.emplace_back(nodeAux);
		}
	}

	for (const vector<int>& entry : aux) {
		generateAllNodesAndEdges(entry, nodesMax,
				         nsize, index);
	}
}
