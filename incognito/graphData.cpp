#include "graphData.h"

GraphData::GraphData() {}

void GraphData::addData(vector<int> data) {
	int nodeId = this->contains(data);
	if (nodeId == -1) {
		this->nodes.emplace_back(this->idCount, data);
		this->idCount++;
	}

}

int GraphData::contains(const vector<int>& node) {
	for (const GraphNode& entry : this->nodes) {
		if (entry.isEqual(node))
			return entry.getId();
	} 
	return -1;
}

void GraphData::generateAllEdges() {
    for (int i=0; i < (int)this->nodes.size(); i++) {
        GraphNode node = this->nodes[i];
        int from=node.getId();
        vector<int> to;
        for (int j=0; j < (int)this->nodes.size(); j++) {
            if (j == i)
                continue;
            if (node.isChild(this->nodes[j]))
                to.emplace_back(j);
		}
        for (const auto& entry : to)
            this->edges.emplace_back(Edge(node.getId(), from, entry));
    }
}

set<GraphNode> GraphData::getRoots() {
	set<GraphNode> roots;
	map<int, int> rootsIds;
	bool flag;

	// Get root nodes ids
	for (const auto& entry1 : this->edges) {
		int  parentId = entry1.getNodeId();
		flag = true;
		for (const auto& entry2 : this->edges) {
			if (entry2.getChild() == parentId) {
				flag = false;
				break;
			}
		}
		if (flag) {
			roots.insert(this->nodes[entry1.getNodeId()]);
		}
	}

	return roots;
}

int GraphData::addGeneralizations(const GraphNode& node, set<GraphNode>& queue) {
	vector<GraphNode> children = getChildren(node);
	if (children.empty())
		return -1;

	for (const GraphNode& child : children) {
		if(!child.marked())
			queue.insert(child);
	}

	return 0;
}

void GraphData::markGeneralizations(const GraphNode& node) {
	this->nodes[node.getId()].setKAnon();

	for (const GraphNode& child : getChildren(node)) {
		this->nodes[child.getId()].mark();
		this->nodes[child.getId()].setKAnon();
	}
}

vector<GraphNode> GraphData::getChildren(GraphNode node) {
	vector<GraphNode> children;

	for (const Edge& edge : this->edges) {
		if (edge.getNodeId() == node.getId()) {
			children.emplace_back(
				this->nodes[edge.getChild()]);
		}
	}
	return children;
}

void GraphData::printAllKAnon(const GraphNode& node, vector<int> kList) {
	for (const GraphNode& node: getChildren(node)) {
		if (find(kList.begin(), kList.end(), node.getId())
			!= kList.end()) {
			node.print();
			kList.emplace_back(node.getId());
			printAllKAnon(node, kList);
		}
	}
}

void GraphData::printAllKAnon() {
	for (const GraphNode& node : this->nodes) {
		if (node.isKAnon()) {
			vector<int> kList;
			node.print();
			printAllKAnon(node, kList);
		}
	}
}

void GraphData::getAllKAnon(const GraphNode& node, vector<int> kList,
							set<GraphNode>& nodes) {
	for (const GraphNode& node: getChildren(node)) {
		if (find(kList.begin(), kList.end(), node.getId())
			!= kList.end()) {
			nodes.insert(node);
			kList.emplace_back(node.getId());
			getAllKAnon(node, kList, nodes);
		}
	}
}

GraphNode GraphData::getFinalKAnon() {
	set<GraphNode> res;
	for (const GraphNode& node : this->nodes) {
		if (node.isKAnon()) {
			vector<int> kList;
			res.insert(node);
			getAllKAnon(node, kList, res);
		}
	}

	return this->nodes[(*res.begin()).getId()];
}

void GraphData::printNodesTable() {
	for (const GraphNode& node : this->nodes) {
		node.print();
	}
}

void GraphData::printEdgesTable() {
	for (const Edge& edge : this->edges) {
		edge.print();
	}
}

