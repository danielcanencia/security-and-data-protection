#include "tree.h"

Tree::Tree(vector<int> nodesMax, vector<int> qids,
	   TreeData& treeData) {
	this->treeData = treeData;
	this->qids = qids;
	generateNodesAndEdges(nodesMax);
}

void Tree::generateNodesAndEdges(const vector<int>& nodesMax) {
	int nsize = nodesMax.size();
	vector<int> inode(nsize, 0);
	//this->treeData.addData(inode,0, 1);
	//this->treeData.addData(inode,0, 2);
	this->treeData.addData(inode);
	//this->treeData.setEdgeChild(edgeFrom, 1);
	//this->treeData.setEdgeChild(edgeFrom, 2);

	int index = 0;
	generateAllNodesAndEdges(inode, nodesMax,
 				 nsize, index);
	this->treeData.generateAllEdges();
	return ;
}


void Tree::generateAllNodesAndEdges(const vector<int>& node,
	   		      const vector<int>& nodesMax,
			      int nsize, int index) {
	vector<vector<int>> aux;

	for (int i=0; i < nsize; i++) {
		vector<int> nodeAux = node;
		if (node[i] < nodesMax[i]) {
			nodeAux[i] += 1;

			// Create node and edge
			// Generate node and edge (we know the parent node)
			this->treeData.addData(nodeAux);

			index += 1;
			aux.emplace_back(nodeAux);
		}
	}

	for (const vector<int>& entry : aux) {
		generateAllNodesAndEdges(entry, nodesMax,
				nsize, index);
		//index++;
	}
}

vector<Node> Tree::getRoots() {
	return this->treeData.getRoots();
}


bool Tree::checkKAnonymity(vector<vector<string>> dataset) {
	/*for (const auto& entry : dataset) {
		for (const auto& val : entry)
			cout << val + ", ";
		cout << endl;
	}*/

	// Iterate through nodes by level
	/*vector firstNode
	vector<Node> children = this->treeData.getChildren();*/
	return true;
}

vector<int> Tree::getQids() const {
	return this->qids;
}

int Tree::addGeneralizations(const Node& node, vector<Node>& queue) {
	return this->treeData.addGeneralizations(node, queue);
}

void Tree::markGeneralizations(const Node& node) {
	this->treeData.markGeneralizations(node);
}

void Tree::printAllKAnon() {
	treeData.printAllKAnon();
}

void Tree::printNodesTable() {
	cout << "***** Nodes Table *****" << endl;
	cout << "Qids: ";
	for (const int& entry : this->qids) {
		cout << to_string(entry) + " ";
	} 
	cout << endl;
	cout << "Table Info: " << endl;
	this->treeData.printNodesTable();
}

void Tree::printEdgesTable() {
	cout << "***** Edges Table *****" << endl;
	this->treeData.printEdgesTable();
}

