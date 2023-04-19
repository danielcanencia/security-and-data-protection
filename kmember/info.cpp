#include "info.h"

Info::Info(vector<vector<string>> records,
           map<int, vector<vector<string>>> hierarchies, map<int, Tree> trees,
           vector<int> numQids, vector<int> catQids) {
  this->records = records;
  this->attsValues = transpose(records);
  this->hierarchies = hierarchies;
  this->trees = trees;
  this->numQids = numQids;
  this->catQids = catQids;

  calculateHeights();
  calculateMaxDomSizes();
}

int Info::valueType(int qid) {
  // If we read a point and no
  // categorical value it must be a
  // numeric value
  if (find(this->numQids.begin(), this->numQids.end(), qid) !=
      this->numQids.end()) {
    return 1;
  }
  if (find(this->catQids.begin(), this->catQids.end(), qid) !=
      this->catQids.end()) {
    return 0;
  }

  return -1;
}

vector<int> Info::getNumQids() { return this->numQids; }

vector<int> Info::getCatQids() { return this->catQids; }

void Info::calculateHeights() {
  int aux = 0;

  for (const auto &idx : this->catQids) {
    int height = 0;
    for (const auto &gens : this->hierarchies[idx]) {
      aux = gens.size();
      if (aux > height)
        height = aux;
    }
    this->treeHeights[idx] = height;
  }
}

void Info::calculateMaxDomSizes() {
  int value, vtype;

  for (size_t i = 0; i < this->attsValues.size(); i++) {
    // Only numeric values
    vtype = valueType(i);
    if (!vtype || vtype == -1)
      continue;

    long double min = stold(this->attsValues[i][0]);
    long double max = stold(this->attsValues[i][0]);

    for (size_t j = 0; j < this->attsValues[i].size(); j++) {
      value = stold(this->attsValues[i][j]);
      if (value > max)
        max = value;
      if (value < min)
        min = value;
    }
    this->maxDomSizes[i] = max - min;
  }
}

int Info::getTreeHeight(int index) { return trees[index].getHeight(); }

long double Info::getMaxDomSize(int index) { return this->maxDomSizes[index]; }

vector<vector<string>> Info::getRecords() { return this->records; }

map<int, vector<vector<string>>> Info::getHierarchies() {
  return this->hierarchies;
}

int Info::getSubTreeHeight(string v1, string v2, int index) {
  string ancestor =
      trees[index].getLowestCommonAncestor(vector<string>({v1, v2})).value;
  return trees[index].getHeight(ancestor);
}

int Info::lowestCommonAncestor(int index) {

  set<string> valueSet;
  vector<int> indexes;
  for (size_t i = 0; i < records.size(); i++) {
    valueSet.insert(records[i][index]);
    indexes.emplace_back(i);
  }

  vector<string> values(valueSet.begin(), valueSet.end());
  string ancestor = trees[index].getLowestCommonAncestor(values).value;

  return trees[index].getHeight(ancestor);
}
