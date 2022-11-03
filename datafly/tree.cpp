#include "tree.h"

Tree::Tree(vector<vector<string>> hierarchy) {

	int child_idx;
	for (const auto& entries : hierarchy) {
		child_idx = addLeave(entries[0], entries[1]);
		/*cout << "Leaf " + entries[0] + " => ";
		auto par = child_idx;*/

		size_t i;
		bool childIsLeaf = true;
		for (i=1; i < entries.size() - 1; i++) {
			//par = child_idx;
			child_idx = addNode(entries[i], childIsLeaf,
					    child_idx, entries[i+1].c_str());

			//cout << child_idx << endl;
			/*if (!childIsLeaf) {
				//cout << this->nodes[child_idx].value + ", ";
				cout << to_string(this->nodes[par].parent) + " == ";
				for (const auto& a : this->nodes[child_idx].children)
					cout << to_string(a) + ", ";
				cout << endl;
			}
			else {
				cout << to_string(this->leaves[par].parent) + " == ";
				for (const auto& a : this->nodes[child_idx].children)
					cout << to_string(a) + ", ";
				cout << endl;
			}*/
			childIsLeaf = false;
		}

		if (entries.size() == 2)
			child_idx = addNode(entries[i], 1, child_idx);
		else
			child_idx = addNode(entries[i], 0, child_idx);

		//cout << this->nodes[child_idx].value + ", ";
		//cout << endl;
	}

}

void Tree::print() {
	int i=0;
	for (const auto& l : this->leaves) {
		cout << "Leaf(" + to_string(i) + "): ";
		cout << l.value + " => " << endl;

		int next = l.parent;
		while (next != -1) {
			cout << this->nodes[next].value << endl;
			next = this->nodes[next].parent;
		}
		cout << endl;
		i++;
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
		if (this->nodes[i].value == value) {
			//cout << value + " == " + this->nodes[i].value << endl;
			index = i;
			break;
		}
	}

	// Node already present
	if (index != -1) {
		// Error add to nodes bool childIsLeaf to know if their childs
		// are leaves
		if (childIsLeaf)
			this->nodes[index].childrenAreLeaves = 1;
		else
			this->nodes[index].childrenAreLeaves = 0;
		this->nodes[index].children.insert(child);

		//cout << this->nodes[index].value + " childs: " + this->nodes[child].value;

		// Set child's parent
		if (childIsLeaf) {
			this->leaves[child].parent = index;
			//cout << to_string(child) + "YEASSS" + to_string(index) << endl;
		}
		else
			this->nodes[child].parent = index;

		return index;
	}

	// Node not present
	// Get parent index
	int pindex = -1;
	if (parent) {
		if (childIsLeaf) {
			for (size_t i=0; i < this->leaves.size(); i++) {
				if (this->leaves[i].value == parent) {
					pindex = i;
					break;
				}
			}
		}	
		else {
			for (size_t i=0; i < this->nodes.size(); i++) {
				if (this->nodes[i].value == parent) {
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
	if (childIsLeaf) {
		this->leaves[child].parent = childIdx;
		//cout << to_string(child) + "YESSS" + to_string(childIdx) << endl;
	}
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

	cout << "Next: " + value + " -> ";
	// It's a leaf
	if (it != this->leaves.end()) {
		cout << "Leaf: ";
        	int idx = it - this->leaves.begin();
		cout << "(i) " + to_string(idx) + " -> ";
		int pidx = this->leaves[idx].parent;
		if (pidx == -1)
			return value;
		cout << to_string(pidx) + " => ";
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
		if (pidx == -1)
			return value;
		return this->nodes[pidx].value;
	}


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


