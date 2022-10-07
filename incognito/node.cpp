#include "node.h"

Node::Node (int id, vector<int> data) {
	this->id = id;
	this->data = data;
}

int Node::getId() const {
	return this->id;
}

int Node::getData(int i) const {
	return this->data[i];
}

bool Node::isEqual (const vector<int>& node) const {
	for (int i=0; i < (int)node.size(); i++) {
		if (this->data[i] != node[i])
			return false;
	}
	return true;
}

bool Node::isChild (Node node) {
	/*int sum1 = node.getSum();
	int sum2 = getSum();

	if (sum1 == 1 + sum2)
		return true;
	return false;	*/

	int sum=0, flag=0;
	for (int i=0; i < this->data.size(); i++) {
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

void Node::print() const {
	cout << "Id: " + to_string(id) + ", ";
	cout << "Data: ";
	for (const int& val : this->data)
		cout << to_string(val) + " ";
	cout << endl;
}

