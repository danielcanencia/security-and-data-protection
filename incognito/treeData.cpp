#include "treeData.h"

TreeData::TreeData() {}

void TreeData::addData(vector<int> data) {
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

void TreeData::generateAllEdges() {
    for (int i=0; i < (int)this->nodes.size(); i++) {
        Node node = this->nodes[i];
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

vector<Node> TreeData::getRoots() {
	vector<Node> roots;
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
			rootsIds[parentId] = entry1.getNodeId();
		}
	}

	// Construct vector of nodes
	for (const auto& entry1 : this->edges)
		roots.emplace_back(this->nodes[entry1.getNodeId()]);

	// Sort nodes by height
	sort(roots.begin(), roots.end());
	return roots;
}

int TreeData::addGeneralizations(const Node& node, vector<Node>& queue) {
	vector<Node> children = getChildren(node);
	if (children.empty())
		return -1;

	for (const Node& child : children) {
		if(!child.marked()) {
			queue.emplace_back(child);
		}
	}

	return 0;
}

void TreeData::markGeneralizations(const Node& node) {
	this->nodes[node.getId()].setKAnon();

	for (Node& child : getChildren(node)) {
		this->nodes[child.getId()].mark();
		this->nodes[child.getId()].setKAnon();
	}
}

vector<Node> TreeData::getChildren(Node node) {
	vector<Node> children;

	for (const Edge& edge : this->edges) {
		if (edge.getNodeId() == node.getId()) {
			children.emplace_back(
				this->nodes[edge.getChild()]);
		}
	}
	return children;
}

void TreeData::printAllKAnon(const Node& node, vector<int> kList) {
	for (const Node& node: getChildren(node)) {
		if (find(kList.begin(), kList.end(), node.getId())
			!= kList.end()) {
			node.print();
			kList.emplace_back(node.getId());
			printAllKAnon(node, kList);
		}
	}
}

void TreeData::printAllKAnon() {
	for (const Node& node : this->nodes) {
		if (node.isKAnon()) {
			vector<int> kList;
			node.print();
			printAllKAnon(node, kList);
		}
	}
}

void TreeData::getAllKAnon(const Node& node, vector<int> kList,
								   vector<Node>& nodes) {
	for (const Node& node: getChildren(node)) {
		if (find(kList.begin(), kList.end(), node.getId())
			!= kList.end()) {
			nodes.emplace_back(node);
			kList.emplace_back(node.getId());
			getAllKAnon(node, kList, nodes);
		}
	}
}

Node TreeData::getFinalKAnon() {
	vector<Node> res;
	for (const Node& node : this->nodes) {
		if (node.isKAnon()) {
			vector<int> kList;
			res.emplace_back(node);
			getAllKAnon(node, kList, res);
		}
	}

	// Choose the 'first' minimal full-domain generalization
	int maxSum = res[0].getSum();
	int nodeIdx = res[0].getId();
	for (size_t idx=1; idx < res.size(); idx++) {
		int sum = res[idx].getSum();
		if (sum < maxSum) {
			maxSum = sum;
			nodeIdx = res[idx].getId();
		}
	}

	//this->nodes[nodeIdx].print();
	return this->nodes[nodeIdx];
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

