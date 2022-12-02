#include "partition.h"

Partition::Partition(vector<vector<string>> data,
		     vector<string> generalizations,
		     vector<int> numSubTreeLeaves,
		     vector<int> qids, vector<Tree> trees,
		     int K) {
	this->data = data;
	this->generalizations = generalizations;
	this->numSubTreeLeaves = numSubTreeLeaves;
	this->qids = qids;
	this->trees = trees;
	this->K = K;
	this->allowedCuts = vector<int>(qids.size(), 1);
}

void Partition::printData() const {
	for (const auto& entry : this->data) {
		for (const auto& a : entry) {
			cout << a + ", ";
		}
		cout << endl;
	}
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

	auto it =
		unique(elements.begin(), elements.end());
	elements.resize(distance(elements.begin(), it));

	cout << elements.size() << endl;
	return elements.size();
}

/*int Partition::getParentDiff(const string x, const string y,
			     int dimension) {
	return trees[dimension].getParentDiff(x, y);
}*/

int Partition::find_median(int dimension) {
	// Calculate qid frecuencies (qid values are sorted)
	//map<vector<int>, int> freqs = calculateQidFreqs(this->data, dimension);
	//map<vector<vector<string>>, int> freqs = calculateQidFreqs(this->data, dimension);
	map<string, int> freqs = calculateQidFreqs(this->data, dimension);


	// **** USAR STRING EN VEZ DE IDX ****
	vector<string> keys;
	// Keys
	transform(freqs.begin(), freqs.end(), std::back_inserter(keys),
		[](const auto &tuple){
			return tuple.first;
		});
	// Sum frequency values
	vector<int> values;
	transform(freqs.begin(), freqs.end(), std::back_inserter(values),
		[](const auto &tuple){
			return tuple.second;
		});
	int nValues = accumulate(values.begin(), values.end(), 0);


	/*cout << "Data: " << endl;
	for (const auto& entry: this->data) {
		for (const auto& x : entry)
			cout << x + ", ";
		cout << endl;
	}
	cout << "Keys: " << endl;
	for (const auto& entry : keys) {
		for (const auto& x : entry) {
			for (const auto& y : x)
				cout << y + ", ";
			cout << endl;
		}
	}
	cout << "***************" << endl;

	cout << "Sorted Keys: " << endl;
	for (const auto& entry : sortedKeys) {
		cout << entry.second; cout << ", ";
	}
	cout << endl;*/

	// Sort numerical attributes
	//*****************************


	// Find middle index 
	int middle = ceil(nValues / 2);
	// Cut is not allowed
	//if (middle < K) {
	//}

	// Find first split set of keys
	int splitIdx;
	int aux = 0;
	//vector<string> split;
	for (size_t i = 0; i < freqs.size(); i++) {
		aux += values[i];
		//split.insert(split.end(), keys[i].begin(),
		//	keys[i].end());
		//split.emplace_back(keys[i]);
		if (aux >= middle) {
			splitIdx = i;
			break;
		}
	}
	/*cout << to_string(aux) + " >= " + to_string(middle) << endl;
	cout << "Split: " << endl;
	for (const auto& entry : split) {
		for (const auto& x : entry) {
			for (const auto& y : x)
				cout << y + ", ";
			cout << endl;
		}
	}*/

	/*
	middleVal = values[middle];
	if (splitIdx + 1 <= values.size())
		nextVal = values[splitIdx];
	else
		nextVal = values[splitIdx + 1];*/

	// Create struct to return
	//return (values[splitIdx], nextVal, values[0], values.back());

	return splitIdx;
}

bool Partition::isCutAllowed(int index) {
	// Cut not allowed if both partitions lengths < K
	cout << "Cut at: " + to_string(index) + ", ";
	cout << ((int)data.size() - index) << endl;
	return ((int)data.size() - index) >= K && index >= K;
}

/*vector<Partition> Partition::cut(vector<int> split,
			int dimension, Tree tree) {

	// Get all values in dimension
	vector<vector<string>> s1;
	//for (const int& idx : split) {
	//	s1.emplace_back(this->data[idx]);
	//}
	vector<vector<string>> s2;
	vector<int> rest;
	for (size_t i=0; i < this->data.size(); i++) {
		if (find(split.begin(), split.end(), i) != split.end())
			s1.emplace_back(this->data[i]);
		else {
			rest.emplace_back(i);
			s2.emplace_back(this->data[i]);
		}
	}

	vector<string> values1, values2;
	for (const int& idx : split)
		values1.emplace_back(this->data[idx]);
	for (const int& idx : rest)
		values2.emplace_back(this->data[idx]);

	// Divide record data by two generalizations.
	// If it's not possible, return unaltered partition. Example: Root => 1, 2, 3
	//array<string> pair = tree.getLowestCommonAncestor(values);
	//if (pair.size() == 1)
	//	return pair[0];	

	// Get lowest common ancestor of every qid value in split
	string a1 = tree.getLowestCommonAncestor(values1).value;
	string a2 = tree.getLowestCommonAncestor(values2).value;

	// If both splits generalizations are the same, return just one 
	if (a1 == a2) {
		vector<string> gens = generalizations;
		gens[dimension] = a1;
		Partition p1(data, gens, qids, K);
		return { p1 };
	}

	//vector<string> values = // Get every x[dim] of pt.data;
        // Split data by common generalization 
        //vector<vector<string>> d1, d2;
	//for (const auto& entry : data) {
	//	if (tree.isGenOf(pair[0], entry[dimension]))
	//		d1.emplace_back(entry);
	//	else
	//		d2.emplace_back(entry);
	//}

	//string ancestor = tree.getLowestCommonAncestor(aux);
	//pt.generalization = ancestor;

	// Update partition generalizations
	vector<string> gens1, gens2;
	gens1 = gens2 = generalizations;
	gens1[dimension] = a1;
	gens2[dimension] = a2;

	Partition p1(s1, gens1, qids, trees, K);
	Partition p2(s2, gens2, qids, trees, K);
	return { p1, p2 };
}
*/

//vector<vector<string>> Partition::evaluate(vector<Tree> trees) {
/*vector<Partition> Partition::evaluate() {
	vector<Partition> result;
	result.emplace_back(this);

	evaluate(result);
	return result;
}*/

vector<Partition> Partition::splitPartitionCategorical(int dimension) {

	vector<Partition> pts;
	Tree tree = trees[dimension];
	string middle = this->generalizations[dimension];
	cout << "Middle: " + middle << endl;
	vector<string> children = trees[dimension].getDirectChildren(middle);
	for (const auto& child : children)
		cout << child + ", ";
	cout << endl;

	// If it doesn't have children, return empty vector
	if (children.size() == 0)
		return pts;

	vector<vector<vector<string>>> splits(children.size(), vector<vector<string>>());
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

	cout << "Splits: " << endl;
	for (const auto& entry : splits) {
		for (const auto& a : entry) {
			for (const auto& b : a)
				cout << b + ", ";
			cout << endl;
		}
		cout << "++++++++++++++++++" << endl;
	}

	//bool flag = true;
	for (const auto& split : splits) {
		if (split.size() > 0 && (int)split.size() < K) {
			return pts;
			//flag = false;
			//break;
		}
	}

	//if (!flag) {
	//	return pts;
	//}

	for (size_t i=0; i < splits.size(); i++) { //const auto& entry : splits) {
		if (splits[i].size() != 0) {
			vector<string> gens = generalizations;
			gens[dimension] = children[i];
			vector<int> numLeaves = numSubTreeLeaves;
			numLeaves[dimension] = tree.getNumSubTreeLeaves(
				children[i]);
			Partition p = Partition(
				splits[i], gens, numLeaves, qids, trees, K);

			pts.emplace_back(p);
		}
	}
	
	return pts;
}

// Si no funciona, usar función fuera de clase, pasando el argumento
// Partition partition
/*void Partition::evaluate(vector<Partition> result) { //, Partition partition) {
	if (accumulate(allowedCuts.begin(),
		   allowedCuts.end(), 0) == 0) {
		//result.emplace_back(partition);
		//return ;
		return ;
	}

	//for (size_t i=0; i < allowedCuts.size(); i++) {
	// Cut not allowed if both partitions lengths < K
	//if (!isCutAllowed())
	//	return dataset;

	int dim = choose_dimension();	// findMostDistinctQid ??
	cout << "Dimension: " + to_string(dim) << endl;

	//Tree tree = trees[dim];

	//int splitIdx = find_median(dim);

	auto pts = splitPartitionCategorical(dim);
	if (pts.size() == 0) {
		//if (!isCutAllowed(split.size()))
		allowedCuts[dim] = 0;
		return ;
	}


	// Cut Partition
	vector<Partition> pts = cut(split, dim, tree);
	// If our splits generalizations are equal, cut is
	// desirable
	if (pts.size() == 1)
		continue;

	// If k-anonimity is not satisfied, dont evalue cuts
	if (!isKAnonSatisfied(pts[0], K) || !isKAnonSatisfied(pts[1], K))
		continue;

	// Evaluate Cuts
	for (const Partition& pt : pts) {
		pt.evaluate(result);
		//for (const auto& partition : pt.evaluate(trees, dataset))
		//	dataset.emplace_back(partition);
	}

	// End loop
	// Other dimensions will be evaluated by children partitions
	return ;

	//result.emplace_back(this);
}*/




