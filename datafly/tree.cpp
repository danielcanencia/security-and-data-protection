#include "tree.h"

Tree::Tree(vector<vector<string>> hierarchy) {

	int child_idx;
	bool childIsLeaf;
	for (const auto& entries : hierarchy) {
		child_idx = addLeave(entries[0], entries[1]);

		size_t i;
		childIsLeaf = true;
		for (i=1; i < entries.size() - 1; i++) {
			child_idx = addNode(entries[i], childIsLeaf,
					    child_idx, entries[i+1].c_str());
			childIsLeaf = false;
		}

		if (entries.size() == 2)
			addNode(entries[i], 1, child_idx);
		else
			addNode(entries[i], 0, child_idx);
	}

}

int Tree::addLeave(string value, string parent) {
	// Check if leave is already present
	for (size_t i=0; i < this->leaves.size(); i++) {
		// If it is, return its index
		if (!strcmp(this->leaves[i].value.c_str(), value.c_str()))
			return i;
	}

	Node node;
	node.value = value;
	this->leaves.emplace_back(node);

	return this->leaves.size() - 1;
}

int Tree::addNode(string value, bool childIsLeaf,
		  int child, const char* parent) {

	// Get node index
	int index = -1;
	for (size_t i=0; i < this->nodes.size(); i++) {
		if (!strcmp(this->nodes[i].value.c_str(), value.c_str())) {
			index = i;
			break;
		}
	}

	// Node already present
	if (index != -1) {
		this->nodes[index].children.emplace_back(child);
		return index;
	}

	// Node not present
	// Get parent index
	int pindex = -1;
	if (parent) {
		cout << parent << endl;
		if (childIsLeaf) {
			for (size_t i=0; i < this->leaves.size(); i++) {
				if (!strcmp(this->leaves[i].value.c_str(), parent)) {
					pindex = i;
					break;
				}
			}
		}	
		else {
			for (size_t i=0; i < this->nodes.size(); i++) {
				if (!strcmp(this->nodes[i].value.c_str(), parent)) {
					pindex = i;
					break;
				}
			}
		}
	}

	// Create new node
	Node node;
	node.value = value;
	node.children = {child};
	node.parent = pindex;
	this->nodes.emplace_back(node);

	// Set child's parent
	int childIdx = this->nodes.size() - 1;
	if (childIsLeaf)
		this->leaves[child].parent = childIdx;
	else
		this->nodes[child].parent = childIdx;

	return this->nodes.size() - 1;
}

struct find_value
{
    string value;
    find_value(const string &value) : value(value) {}
    bool operator () (const Node& v) const
    {
        return v.value == value;
    }
};

string Tree::getNextGen(string value) {
	auto it = find_if(this->leaves.begin(),
		   	   this->leaves.end(),
			   find_value(value));

	cout << "Next: " + value << endl;
	// It's a leaf
	if (it != this->leaves.end()) {
		cout << "Leaf" << endl;
        	int idx = it - this->leaves.begin();
		int pidx = this->leaves[idx].parent;
		return this->nodes[pidx].value;
	}

	// It's not
	it = find_if(this->nodes.begin(),
	   	     this->nodes.end(),
		     find_value(value));

	if (it != this->nodes.end()) {
		cout << "Node" << endl;
		int idx = it - this->nodes.begin();
		int pidx = this->nodes[idx].parent;
		return this->nodes[pidx].value;
	}

	cout << "as" << endl;

	// Not found
	throw "Error: Element not found in the tree";
}

/*
vector<string> Tree::getNextGens(vector<string> values) {

	vector<string> gens;

	// Get the highest level of anonymization between
	// all strings
	int level = getMaxGenLevel(values);

	for (const string& s : values) {
		gens.emplace_back(generalize(s, level));
	}

	return gens;
}*/


