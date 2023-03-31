#ifndef _GRAPHNODE_H
#define _GRAPHNODE_H

#include "../utilities/frequencies.h"
#include "../utilities/hierarchy.h"
#include "anonymity.h"
#include <bits/stdc++.h>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

class GraphNode {
private:
  int id;
  vector<int> data;
  bool nodeMark = false;
  bool kAnon = false;
  string generalizeEntry(string entry, const vector<vector<string>> hierarchy,
                         vector<string> generalizations);

public:
  GraphNode();
  GraphNode(int id, vector<int> data);
  int getId() const;
  int getData(int i) const;
  vector<int> getData() const;
  bool isChild(GraphNode node);
  bool isEqual(const vector<int> &node) const;
  bool operator<(const GraphNode &node) const;
  int getSum() const;
  bool marked() const;
  void mark();
  void setKAnon();
  bool isKAnon() const;
  bool isAnonymityValid(map<int, vector<vector<string>>> hierarchies,
                        vector<vector<string>> dataset,
                        map<int, map<string, vector<string>>> gensMap,
                        vector<int> qids, vector<int> confAtts, const int K,
                        const int L, const long double P);
  vector<int>
  evaluateFrequency(map<int, map<string, vector<string>>> generalizations,
                    vector<vector<string>> dataset, vector<int> qids) const;
  void print() const;
};

#endif
