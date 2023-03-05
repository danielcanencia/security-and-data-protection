#include "graphNode.h"

GraphNode::GraphNode () {
	this->id = -1;
};

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

bool GraphNode::isAnonymityValid(map<int, vector<vector<string>>> hierarchies,
			 vector<vector<string>> dataset,
			 map<int, map<string, vector<string>>> gensMap,
			 vector<int> qids, vector<int> confAtts,
			 const int K, const int L, const long double P) {

	// Map every unique combination of qids to a matrix/cluster;
	map<string, vector<vector<string>>> splits;
	vector<vector<string>> anonData;
	string value, choosenGen;
	for (const auto& entry : dataset) {
		string value;
		vector<string> record = entry;
		for (size_t i=0; i < qids.size(); i++) {
			choosenGen = gensMap[qids[i]][entry[qids[i]]][this->data[i]];
			value.append(choosenGen);
			record[qids[i]] = choosenGen;
		}

		try {
			splits[value].emplace_back(record);
		} catch (...) {
			splits[value] = vector<vector<string>>(1, record);
		}

		anonData.emplace_back(record);
	}

	// Create clusters from map values
	vector<vector<vector<string>>> clusters;
	for (const auto& [k, v] : splits)
		clusters.emplace_back(v);

	return isSplitValid(clusters, anonData, confAtts, K, L, P);
}

vector<int> GraphNode::evaluateFrequency(
			map<int, map<string, vector<string>>> generalizations,
	 		vector<vector<string>> dataset, vector<int> qids) const {

	// Construct anonymized column for one qid
	vector<string> genData;
	int qid = qids[0];
	for (size_t i=0; i < dataset.size(); i++) {
		genData.emplace_back(generalizations[qid][dataset[i][qid]][data[0]]);
	}

	// Check frequencies for this specific qid
	map<string, int> freqMap;
	for (const auto& gen : genData) {
		// Need to check for just one qid
		try {
			freqMap[gen] += 1;
		} catch (...) {
			freqMap[gen] = 1;
		}
	}

	// Get list of values
	vector<int> freqs;
	for (const auto& [k, v] : freqMap)
		freqs.emplace_back(v);

	return freqs;
}

void GraphNode::print() const {
	cout << "Id: " + to_string(id) + ", ";
	cout << "Data: ";
	for (const int& val : this->data)
		cout << to_string(val) + " ";
	cout << endl;
}

