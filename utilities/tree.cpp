#include "tree.h"
#include <iostream>

Tree::Tree() { };

Tree::Tree(vector<vector<string>> hierarchy) {

	for (const auto& entries : hierarchy) {
		addLeave(entries[0], entries[1], entries.size()-1);

		size_t i;
		string child = entries[0];
		for (i=1; i < entries.size() - 1; i++) {
			child = addNode(entries[i], child,
					entries[i+1].c_str());
		}

		addNode(entries[i], child);
	}

}

void Tree::addLeave(string value, string parent, int depth) {
	// Check if leave is already present
	if (this->nodes.count(value))
		return ;

	Node node;
	node.value = value;
	node.parent = parent;
	node.isLeaf = true;
	node.depth = depth;
	this->nodes[value] = node;

	this->leaves.emplace_back(value);
}

string Tree::addNode(string value, string child, const char* parent) {

	// Node already present
	if (this->nodes.count(value)) {
		this->nodes[value].children.insert(child);
		this->nodes[value].numSubTreeLeaves += 1;
		return value;
	}

	// Node not present
	Node node;
	node.value = value;
	if (parent)
		node.parent = (string)parent;
	else {
		this->root = value;
		node.parent = value;
	}

	node.depth = this->nodes[child].depth - 1;
	node.children = {child};
	node.numSubTreeLeaves += 1;
	this->nodes[value] = node;

	return value;
}

int Tree::getNumSubTreeLeaves(string value) {
	return this->nodes[value].numSubTreeLeaves;
}


int Tree::getDepth(string value) {
	return this->nodes[value].depth;
}

bool Tree::isChild(string node, string target) {
	set<string> children = this->nodes[node].children;

	if (find(children.begin(), children.end(), target)
		!= children.end())
		return true;

	for (const auto& child : children) {
		if (isChild(child, target))
			return true;
	}

	return false;
}

vector<string> Tree::getDirectChildren(string value) {
	set<string> children = this->nodes[value].children;
	return vector<string>(children.begin(), children.end());
}

vector<string> Tree::getAllChildren(string value) {
	vector<string> children;

	for (const auto& child : this->nodes[value].children) {
		children.emplace_back(child);
		vector<string> next = getAllChildren(child);
		children.insert(children.begin(), next.begin(), next.end());
	}

	return children;
}


string Tree::getNextGen(string value) {

	// Node present 
	if (this->nodes.count(value)) {
		string parent = this->nodes[value].parent;
		if (parent == value)
			return value;

		return parent;
	}
	cout << value << endl;
	// Not found
	throw "Error: Element not found in the tree";
}

int Tree::getNumLeaves() {
	return this->leaves.size();
}

Node Tree::getCommonAncestor(Node node, string target) {
	if (node.value == target)
		return node;

	// It is a direct child
	if (find(node.children.begin(), node.children.end(),
		 target) != node.children.end())
		return node;

	Node n = this->nodes[node.parent];
	vector<string> children(node.children.begin(),
				node.children.end());

	while (find(children.begin(), children.end(), target)
		== children.end()) {

		n = this->nodes[n.parent];
		children.insert(children.begin(),
			n.children.begin(), n.children.end());
	}

	return n;
}

Node Tree::getLowestCommonAncestor(string r1, string r2) {

	Node n1 = this->nodes[r1];
	Node n2 = this->nodes[r2];
	Node node;
	string target;

	if (r1 == r2)
		return n1;

	if (this->nodes[n1.parent].numSubTreeLeaves < 
		this->nodes[n2.parent].numSubTreeLeaves) {
		node = this->nodes[n1.parent];
		target = n2.value;
	}
	else {
		node = this->nodes[n2.parent];
		target = n1.value;
	}

	return getCommonAncestor(node, target);
}

Node Tree::getLowestCommonAncestor(vector<string> values) {
	Node node;
	string aux = values[0];

	if (values.size() == 1)
		return this->nodes[aux];

	for (size_t i=1; i < values.size(); i++) {
		node = getLowestCommonAncestor(aux, values[i]);
		// Return root node if we've reached it 
		if (node.parent == node.value)
			return node;
		aux = node.value;
	}

	return node;
}

long double Tree::getNCP(vector<string> values) {
	// Get unique values only
	vector<string> aux = values;
	vector<string>::iterator it =
		unique(aux.begin(), aux.end());
	aux.resize(distance(aux.begin(), it));

	int subTreeLeaves = getLowestCommonAncestor(aux).numSubTreeLeaves;
	long double ncp = (long double)subTreeLeaves;
	return ncp;
}

