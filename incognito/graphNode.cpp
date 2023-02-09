#include "graphNode.h"

GraphNode::GraphNode (int id, vector<int> data) {
	this->id = id;
	this->data = data;
}

int GraphNode::getId() const {
	return this->id;
}

int GraphNode::getData(int i) const {
	return this->data[i];
}

vector<int> GraphNode::getData() const {
	return this->data;
}

bool GraphNode::isEqual(const vector<int>& node) const {
	for (int i=0; i < (int)node.size(); i++) {
		if (this->data[i] != node[i])
			return false;
	}
	return true;
}

bool GraphNode::isChild(GraphNode node) {

	int sum=0, flag=0;
	for (int i=0; i < int(this->data.size()); i++) {
		if (this->data[i] == node.getData(i) - 1) {
			sum = sum + 1;
		}
		if (abs(node.getData(i) - this->data[i]) == 1) {
			flag += 1;
		}

		if (abs(node.getData(i) - this->data[i]) >= 2) {
			return false;
		}
	}

	if (sum == 1 && flag == 1)
		return true;
	return false;
}

bool GraphNode::operator < (const GraphNode& node) const {
	return (getSum() < node.getSum()) || (id < node.getId());
}

int GraphNode::getSum() const {
	int sum = 0;
	for (const auto& entry : this->data)
		sum += entry;
	return sum;
}

bool GraphNode::marked() const {
	return this->nodeMark;
}

void GraphNode::mark() {
	this->nodeMark = true;
}

void GraphNode::setKAnon() {
	this->kAnon = true;
}

bool GraphNode::isKAnon() const {
	return this->kAnon;
}

string GraphNode::generalizeEntry(string entry, const vector<vector<string>> hierarchy,
		       vector<string> generalizations) {
	int index = -1;

	for (int i=0; i < int(hierarchy.size()); i++) {
		for (int j=0; j < int(generalizations.size()); j++) {
			if (hierarchy[i][j].compare(entry) == 0) {
				index = j; break;
			}
		}
	}

	if (index == -1)
		return "No entry found";

	return generalizations[index];
}

vector<int> GraphNode::evaluateFrequency(vector<vector<string>> generalizations,
	    int rows, int cols) {
	// Transpose generalizations matrix
	// && Concat values in every row
	string genT[cols][rows];
	vector<stringstream> values(cols);
	for (int i=0; i < rows; i++) {
		for (int j=0; j < cols; j++) {
			genT[j][i] = generalizations[i][j];
			values[j] << genT[j][i];
		}
	}

	// Convert stringstream array to an array of string
	map<string, int> vals;

	for (auto a = values.begin(); a != values.end(); a++)
		vals[(*a).str()] = 1;

	// Get map keys
    vector<string> keys;
    for (const auto& [k, v] : vals)
	    keys.push_back(k);
 
	// Count unique values
	vector<int> count(keys.size());
	// Check frecuencies in values array
	for (size_t i=0; i < keys.size(); i++) {
		for (int j=0; j < cols; j++) {
			if (keys[i] == values[j].str()) {
				count[i]++;
			}
		}
	}

	return count;
}

bool GraphNode::getKAnonymity(map<int, vector<vector<string>>> hierarchies,
			 vector<vector<string>> transposedTable,
			 map<int, map<string, vector<string>>> gensMap,
			 vector<int> qids, int K) {

	vector<vector<string>> genArray(qids.size());

	// Iterate through every table row corresponding to node's
	// qids attributes
	int index = 0;
	for (const int& qid : qids) {
		// Table T rows representing qid's values
		for (const string& entry : transposedTable[qid]) {
			// Generate new row generalize

			// Iterate through possible generalizations in this
			// level and find the one that suits the original data.
			string choosenGen = gensMap[qid][entry][this->data[index]]; 
			//choosenGen = generalizeEntry(entry, hierarchies[qid], generalizations);
			genArray[index].emplace_back(choosenGen);
		}
		index++;
	}

	// Evaluate Frecuency set of T with respect to attributes of node (qids)
	vector<int> freq = evaluateFrequency(genArray, index, transposedTable[0].size());

	int flag = 1;
	for (const int& value : freq)
		if (value < K) flag = 0;
	if (!flag)
		return false;

	return true;
}


void GraphNode::print() const {
	cout << "Id: " + to_string(id) + ", ";
	cout << "Data: ";
	for (const int& val : this->data)
		cout << to_string(val) + " ";
	cout << endl;
}

