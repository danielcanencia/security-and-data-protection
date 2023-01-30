#include "partition.h"

Partition::Partition(vector<vector<string>> data,
		     vector<string> generalizations,
		     vector<int> qids, vector<int> isQidCat,
		     map<int, Tree> trees, int K) {
	this->data = data;
	this->generalizations = generalizations;
	this->qids = qids;
	this->isQidCat = isQidCat;
	this->trees = trees;
	this->K = K;
	this->allowedCuts = vector<int>(qids.size(), 1);
}

vector<vector<string>> Partition::getResult() const {
	vector<vector<string>> result = data;

	for (size_t i=0; i < data.size(); i++) {
		for (size_t j=0; j < qids.size(); j++) {
			result[i][j] = generalizations[j];
		}
	}

	return result;
}

int Partition::getNumAllowedCuts() {
        return accumulate(allowedCuts.begin(),
                   allowedCuts.end(), 0);
}

void Partition::setAllowedCuts(int value, int dim) {
	allowedCuts[dim] = value;
}


int Partition::choose_dimension() {
	int dimension, width;
	dimension = width = -1;

	for (size_t i = 0; i < qids.size(); i++) {
		if (allowedCuts[i] == 0)
			continue;

		int aux = normWidth(i);
		if (aux > width) {
			width = aux;
			dimension = i;
		}
	}

	return dimension;
}

int Partition::normWidth(int dimension) {
	// Normalized range of values for dimension
	vector<string> elements;
	for (const vector<string>& entry : data) {
		elements.emplace_back(entry[dimension]);
	}

	auto it = unique(elements.begin(), elements.end());
	elements.resize(distance(elements.begin(), it));

	return elements.size();
}

string Partition::findMedian(int dimension) {
	// Calculate qid frecuencies (qid values are sorted)
	map<string, int> map = calculateQidFreqs(this->data, dimension);
	vector<pair<string, int>> freqs;
	freqs.resize(map.size());
	copy(map.begin(), map.end(), freqs.begin());

	// Sort values
	sort(freqs.begin(), freqs.end(),
		[](const pair<string, int>& x,
		   const pair<string, int>& y) {
			return stoi(x.first) < stoi(y.first);
		});

	// Sum frequency values
	vector<int> values;
	transform(freqs.begin(), freqs.end(), std::back_inserter(values),
		[](const auto &tuple){
			return tuple.second;
		});
	int nValues = accumulate(values.begin(), values.end(), 0);

	// Find middle index 
	double middle = nValues / 2.0;
	// Cut is not allowed
	if (freqs.size() < 2 || middle < K) {
		return "";
	}

	// Find first split set of keys
	string splitValue;
	int aux = 0;
	for (size_t i = 0; i < freqs.size(); i++) {
		aux += freqs[i].second;
		if (aux >= middle) {
			splitValue = freqs[i].first;
			break;
		}
	}

	return splitValue;
}

/*
bool Partition::isCutAllowed(int index) {
	// Cut not allowed if both partitions lengths < K
	cout << ((int)data.size() - index) << endl;
	return ((int)data.size() - index) >= K && index >= K;
}*/

vector<int> Partition::getAttributeRanges(int dimension) {
	vector<int> values;
	for (const auto& record : this->data) {
		values.emplace_back(stoi(record[dimension]));
	}

	int min = *min_element(values.begin(), values.end());
	int max = *max_element(values.begin(), values.end());

	return {min, max}; 
}

vector<Partition> Partition::splitPartition(int dimension) {
	// Cut is not allowed for this specific dimension
	if (allowedCuts[dimension] == 0)
		return {};

	if (isQidCat[dimension])
		return splitPartitionCategorical(dimension);
	return splitPartitionNumeric(dimension);
}

vector<Partition> Partition::splitPartitionNumeric(int dimension) {
	string splitValue = findMedian(dimension);

	// Cut is not allowed
	if (splitValue == "") {
		/*Partition p1(this->data, this->generalizations,
			     qids, isQidCat, trees, K);
		return { p1 };*/
		return { };
	}

	// Check if splitValue is a range
	string split1, split2;
	/*int index = splitValue.find('~');
	if (index == (int)string::npos) {*/
		split1 = splitValue;
		split2 = to_string(stoi(splitValue) + 1);
	/*}
	else {
		// Split ranged value in two
		string v1 = splitValue.substr(0, index - 1);
		string v2 = splitValue.substr(index + 1, splitValue.size());
		if (v1 == v2)
			split1 = split2 = v1;
		else {
			split1 = v1;
			split2 = v2;
		}
	}*/

	// Get lowest and highest value present in partition,
	// Note that ranges are not a problem if not present
	// initial data, as we don't modify these values, just
	// this->generalizations value.
	string lowest, highest;
	vector<int> range = getAttributeRanges(dimension);
	lowest = to_string(range[0]);
	highest = to_string(range[1]);
	
	string gen1 = lowest==splitValue ? splitValue : lowest + "~" + split1;
	string gen2 = highest==splitValue ? splitValue : split2 + "~" + highest;

	// New partitions data
	vector<vector<string>> d1, d2;
	for (const auto& record : this->data) {
		int number = stoi(record[dimension]);
		if (number <= stoi(split1))
			d1.emplace_back(record);
		else
			d2.emplace_back(record);
	}

	// Updata generalizations array with new dimension values
	vector<string> gens1, gens2;
	gens1 = gens2 = this->generalizations;
	gens1[dimension] = gen1;
	gens2[dimension] = gen2;
	Partition p1(d1, gens1, qids, isQidCat, trees, K);
	Partition p2(d2, gens2, qids, isQidCat, trees, K);

	return { p1, p2 };
}

vector<Partition> Partition::splitPartitionCategorical(int dimension) {

	vector<Partition> pts;
	Tree tree = trees[dimension];
	string middle = this->generalizations[dimension];
	vector<string> children = tree.getDirectChildren(middle);

	// If it doesn't have children, return empty vector
	if (children.size() == 0)
		return pts;

	// Split records based on dimension (qid) generalization values
	vector<vector<vector<string>>> splits(children.size(),
	       vector<vector<string>>());
	for (const auto& record : this->data) {
		string qid = record[dimension];

		int i=0;
		for (const auto& child : children) {
			if (!tree.isChild(child, qid)) {
				i++;
				continue;
			}
			splits[i].emplace_back(record);
			break;
		}
	}

	// If splits aren't k-anonymous, return original partition
	for (const auto& split : splits) {
		if (split.size() > 0 && (int)split.size() < K) {
			return pts;
		}
	}

	// Update generalizations and create new partitions
	for (size_t i=0; i < splits.size(); i++) {
		if (splits[i].size() != 0) {
			vector<string> gens = generalizations;
			gens[dimension] = children[i];
			Partition p = Partition(splits[i], gens,
					qids, isQidCat, trees, K);

			pts.emplace_back(p);
		}
	}
	
	return pts;
}

