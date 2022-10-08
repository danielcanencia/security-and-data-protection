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

vector<Node> TreeData::getRoots() {
	vector<Node> roots;
	vector<int> rootsIds;
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
		if (flag)
			rootsIds.emplace_back(entry1.getNodeId());
	}

	for (const auto& v : rootsIds)
		cout << v;
	cout << endl;

	// Remove unique values nodeIds
	sort(rootsIds.begin(), rootsIds.end());
	rootsIds.erase(unique(rootsIds.begin(), rootsIds.end()), rootsIds.end());

	// Construct vector of nodes
	for (size_t i=0; i < rootsIds.size(); i++)
		roots.emplace_back(this->nodes[rootsIds[i]]);

	// Sort nodes by height
	sort(roots.begin(), roots.end());

	return roots;
}

int TreeData::addGeneralizations(const Node& node, vector<Node>& queue) {
	vector<Node> children = getChildren(node);
	if (children.empty())
		return -1;

	cout << "Add generalizations: " << endl;
	for (const Node& child : children) {
		if(!child.marked()) {
			child.print();
			queue.emplace_back(child);
		}
	}

	return 0;
}

void TreeData::markGeneralizations(const Node& node) {
	cout << "Marking....." << endl;
	for (Node& child : getChildren(node)) {
		child.print();
		this->nodes[child.getId()].mark();
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

