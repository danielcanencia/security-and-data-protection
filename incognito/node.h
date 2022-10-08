#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <vector>
#include <numeric>
#include <bits/stdc++.h>
#include <sstream>

using namespace std;


class Node {
private:
	int id;
	vector<int> data;
	bool nodeMark = false;

	string generalizeEntry(string entry, const vector<vector<string>> hierarchy,
                       vector<string> generalizations);
	vector<int> evaluateFrequency(vector<vector<string>> generalizations,
	 		       int rows, int cols);

public:
	Node (int id, vector<int> data);
	int getId() const;
	int getData(int i) const;
	bool isChild(Node node);
	bool isEqual (const vector<int>& node) const;
	bool operator < (const Node& node) const;
	int getSum() const;
	bool marked() const;
	void mark(); 

	bool getKAnonymity(vector<vector<vector<string>>> hierarchies,
                           vector<vector<string>> transposedTable,
                           vector<vector<string>> table,
                           vector<int> qids, int K);
	void print() const;
};

#endif
