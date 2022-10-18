#include "info.h"

int Info::valueType(int qid) {
        // If we read a point and no
        // categorical value it must be a
        // numeric value
	if (find(this->numQids.begin(),
		 this->numQids.end(), qid)
		!= this->numQids.end()) {
		return 1;
	}
	if (find(this->catQids.begin(),
		 this->catQids.end(), qid)
		!= this->catQids.end()) {
		return 0;
	}	

	return -1;
}

Info::Info (vector<vector<string>> records,
     	    map<int, vector<vector<string>>> hierarchies,
     	    vector<int> numQids, vector<int> catQids) {
	this->records = records;
	this->attsValues = transpose(records);
	this->hierarchies = hierarchies;
	this->numQids = numQids;
	this->catQids = catQids;

/*
	cout << "catQIDS: ";
	for (const auto& entry : this->catQids) {
		cout << to_string(entry) + ": ";
		cout << records[entry][0] + ": ";
		cout << hierarchies[entry][0][0] + ", ";
	}
	cout << endl;
*/
	calculateHeights();
	calculateMaxDomSizes();
	/*
	for (const auto& entry : this->numQids)
		cout << to_string(this->maxDomSizes[entry]) + ", ";
	cout << endl;
	for (const auto& entry : this->catQids)
		cout << to_string(this->treeHeights[entry]) + ", ";
	cout << endl;*/
} 

void Info::calculateHeights() {
	int aux=0;

	for (const auto& idx : this->catQids) {
		int height = 0;
		for (const auto& gens : this->hierarchies[idx]) {
			cout << gens.size();
			aux = gens.size();
			if (aux > height)
				height = aux;
		}
		this->treeHeights[idx] = height;
	}
}

void Info::calculateMaxDomSizes() {
	int value, vtype;

	for (size_t i=0; i < this->attsValues.size(); i++) {
		// Only numeric values
		vtype = valueType(i);
		if (!vtype || vtype == -1)
			continue;

		long double min = stold(this->attsValues[i][0]);
		long double max = stold(this->attsValues[i][0]);

		for (size_t j=0; j < this->attsValues[i].size(); j++) {
			value = stold(this->attsValues[i][j]);
			if (value > max)
				max = value;
			if (value < min)
				min = value;
		}
		this->maxDomSizes[i] = max - min;
	}
}

int Info::getSubTreeHeight(string v1, string v2,
		  	   int attIndex) {
	int flag = 0, len1 = 0, len2 = 0;
	vector<vector<string>> res;

	for (const auto& entry : this->hierarchies[attIndex]) { 
		for (size_t i=0; i < entry.size(); i++) {
			//cout << entry[i] + ", ";
			if (!strcmp(v1.c_str(), entry[i].c_str())) {
				vector<string> aux(entry.begin() + i+1, entry.end());

				res.emplace_back(move(aux));
				flag += 1;
				if (flag==1)
					len1 = i+1;
				else
					len2 = i+1;
			}
			if (!strcmp(v2.c_str(), entry[i].c_str())) {
				vector<string> aux(entry.begin() + i+1, entry.end());

				res.emplace_back(move(aux));
				flag += 1;
				if (flag==1)
					len1 = i+1;
				else
					len2 = i+1;
			}
		}
	}
	if (flag==2) {
		for (int i=0; i < (int)res[0].size(); i++) {
			for (int j=0; j < (int)res[1].size(); j++) {
				if (!strcmp(res[0][i].c_str(), res[1][j].c_str())) {
					return max(len1 + i, len2 + j);
				}
			}
		}
	}

	return -1;
}

int Info::getTreeHeight(int index) {
	return this->treeHeights[index];
}

long double Info::getMaxDomSize(int index) {
	return this->maxDomSizes[index];
}

vector<vector<string>> Info::getRecords() {
	return this->records;
}

map<int, vector<vector<string>>> Info::getHierarchies() {
	return this->hierarchies;
}

int Info::lowestCommonAncestor(int index) {

	int[this->records.size()] indexes;
	for (size_t i=0; i < records.size; i++)
		indexes[i] = i;

	// Get all permutations of records
	vector<vector<int>> perms = getPermutations(
		2, indexes);

	for (const auto& perm : perms) {
		int aux, height = 0;
		aux = getSubTreeHeight(
			this->records[perm[0]],
			this->records[perm[1]],
			index);

		if (aux > height)
			height = aux;
	}

	return height;
}

