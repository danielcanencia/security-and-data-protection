#include "treeData.h"

TreeData::TreeData() {}

void TreeData::addData(vector<int> data) {
	/*int nodeId = this->contains(node);
	if (nodeId == -1) {
		this->nodes.emplace_back(this->idCount,
					 data);
		//this->edges.emplace_back(
		//	Edge(this->idCount, from, to));
		return ;
	}*/
	int nodeId = this->contains(data);
	if (nodeId == -1) {
		this->nodes.emplace_back(this->idCount, data);
		this->idCount++;
	}

}

int TreeData::contains(const vector<int>& node) {
	for (const Node& entry : this->nodes) {
		if (entry.isEqual(node))
			return entry.getId();
	} 
	return -1;
}

int TreeData::contains(int from, int to) {
	for (const Edge& entry : this->edges) {
		if (entry.isEqual(from, to))
			return true;
	} 
	return false;
}

void TreeData::generateAllEdges() {
        for (int i=0; i < (int)this->nodes.size(); i++) {
                Node node = this->nodes[i];
                int from=node.getId();
                vector<int> to;
                for (int j=0; j < (int)this->nodes.size(); j++) {
                        if (j == i)
                                continue;
                        //if (node.isParent(this->nodes[j]))
                        //        from = j;
                        if (node.isChild(this->nodes[j]))
                                to.emplace_back(j);
		}
		//if (to.size() == 0)
		//	this->edges.emplace_back(Edge(node.getId(), from, node.getId()));
                for (const auto& entry : to)
                        this->edges.emplace_back(Edge(node.getId(), from, entry));
        }

}

void TreeData::printNodesTable() {
	for (const Node& node : this->nodes) {
		node.print();
	}
}

void TreeData::printEdgesTable() {
	for (const Edge& edge : this->edges) {
		edge.print();
	}
}

