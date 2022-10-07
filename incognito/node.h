#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <vector>
#include <numeric>


using namespace std;


class Node {
private:
	int id;
	vector<int> data;
public:
	Node (int id, vector<int> data);
	int getId() const;
	int getData(int i) const;
	bool isChild(Node node);
	bool isEqual (const vector<int>& node) const;
	void print() const;
};

#endif
