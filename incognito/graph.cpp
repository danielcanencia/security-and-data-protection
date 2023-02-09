#include "graph.h"

Graph::Graph() { };

Graph::Graph(vector<int> nodesMax, vector<int> qids,
	   GraphData& graphData) {
	this->graphData = graphData;
	this->qids = qids;
	generateNodesAndEdges(nodesMax);
}

void Graph::generateNodesAndEdges(const vector<int>& nodesMax) {
	int nsize = nodesMax.size();
	vector<int> inode(nsize, 0);
	this->graphData.addData(inode);

	int index = 0;
	generateAllNodesAndEdges(inode, nodesMax,
 				 nsize, index);
	this->graphData.generateAllEdges();
	return ;
}

void Graph::generateAllNodesAndEdges(const vector<int>& node,
	   		      const vector<int>& nodesMax,
			      int nsize, int index) {
	vector<vector<int>> aux;

	for (int i=0; i < nsize; i++) {
		vector<int> nodeAux = node;
		if (node[i] < nodesMax[i]) {
			nodeAux[i] += 1;

			// Create node and edge
			// Generate node and edge (we know the parent node)
			this->graphData.addData(nodeAux);

			index += 1;
			aux.emplace_back(nodeAux);
		}
	}

	for (const vector<int>& entry : aux) {
		generateAllNodesAndEdges(entry, nodesMax,
				nsize, index);
	}
}

set<GraphNode> Graph::getRoots() {
	return this->graphData.getRoots();
}


vector<int> Graph::getQids() const {
	return this->qids;
}

int Graph::addGeneralizations(const GraphNode& node, set<GraphNode>& queue) {
	return this->graphData.addGeneralizations(node, queue);
}

void Graph::markGeneralizations(const GraphNode& node) {
	this->graphData.markGeneralizations(node);
}

void Graph::printAllKAnon() {
	graphData.printAllKAnon();
}

GraphNode Graph::getFinalKAnon() {
	return graphData.getFinalKAnon();
}

void Graph::printNodesTable() {
	cout << "***** Nodes Table *****" << endl;
	cout << "Qids: ";
	for (const int& entry : this->qids) {
		cout << to_string(entry) + " ";
	} 
	cout << endl;
	cout << "Table Info: " << endl;
	this->graphData.printNodesTable();
}

void Graph::printEdgesTable() {
	cout << "***** Edges Table *****" << endl;
	this->graphData.printEdgesTable();
}

