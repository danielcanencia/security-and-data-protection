#include <iostream>
#include "distances.h"

int randomRecord(vector<vector<string>> records) {
	srand(time(NULL));
	int random = rand() % records.size();
	return random;
}

int furthestRecord(const vector<string> record,
	      	   int index,
		   const vector<vector<string>> records,
		   const map<int, vector<vector<string>>> hierarchies,
		   const vector<int> numQids,
		   const vector<int> catQids) {
	long double furthestDiff = -1;
	int furthestIdx = -1;


	// Precalculate all needed values
	Info info(records, hierarchies, numQids, catQids);


	// Compare current record with all records
	// except himself
	for (size_t idx = 0; idx < records.size(); idx++) {
		if ((int)idx == index)
			continue;

		// Compute distance between the two records
		const long double aux =
			distance(record, records[idx],
				 info);
		if (aux > furthestDiff || !furthestDiff) {
			furthestDiff = aux;
			furthestIdx = idx;
		}

		cout << "Furthest: " << endl;
		cout << furthestIdx << endl;
	}

	// Must fail if records contains just
	// the record 'record'
	//if (furthestIdx == -1)
 	//	return -1;


	return furthestIdx;
}

long double numDistance(long double v1,
			long double v2,
			long double maxDomSize) {

	return abs(v1 - v2) / maxDomSize;
}

long double catDistance(const string& v1, const string& v2,
			Info info, int attIndex) {

	return info.getSubTreeHeight(v1, v2, attIndex) /
	       info.getTreeHeight(attIndex);

	return 1;
}
	
long double distance(const vector<string>& r1,
		     const vector<string>& r2,
		     Info info) {
	long double numSum = 0.0, catSum = 0.0;
	string v1, v2;
	int vtype;

	// Iterate through every qid attribute
	for (size_t idx=0; idx < r1.size(); idx++) {
		v1 = r1[idx];
		v2 = r2[idx];
		vtype = info.valueType((int)idx);
		cout << v1 + ", " + v2 << endl;


		// Check if value is numeric or categorical
		if (vtype) {
			// Numeric value
			numSum += numDistance(stold(v1),
					stold(v2),
					info.getMaxDomSize(idx));
		}
		else if (!vtype) {
			// Categorical value
			catSum += catDistance(v1, v2, info, idx);
		}
	}

	return numSum + catSum;
}

long double information_loss(vector<vector<string>> records,
		   	     const vector<int> numQids,
		   	     const vector<int> catQids) {
	int e = records.size();
	vector<long double> numValues, catValues;

	// Numeric Attributes
	for (const int& idx : numQids) {
		long double max, min, aux;
		max = min = -1;
		vector<long double> numericDomain;	
		for (const vector<string>& record : records) { //size_t i=0; i < records.size(); i++) {
			aux = stold(record[idx]);

			numericDomain.emplace_back(aux);	

			if (aux > get<0>(tp))
				get<0>(tp) = aux;
			if (aux < get<1>(tp) || get<1>(tp) == -1)
				get<1>(tp) = aux;
		}

		int domainSize = std::unique(numericDomain.begin(),
				     numericDomain.end()).size();

		numValues.emplace_back((max - min) / domainSize); 
	}
  
	// Categorical Attributes
	Info info(records, hierarchies, vector<int>{}, catQids);

	for (const int& idx : catQids) {
		catValues.emplace_back(
			info.lowestCommonAncestor() /
			info.getTreeHeight(idx));
	}


	// Sum all values in every vector
	auto r1 = reduce(numValues.begin(), numValues.end());
	auto r2 = reduce(catValues.begin(), catValues.end());

	return r1 + r2;
}

int find_best_record(vector<vector<string>> records,
		     vector<vector<string>> cluster) {
	size_t nclusters = cluster.size();
	long double min = -1, diff;
	int best;

	for (size_t i=0; i < nclusters; i++) {
		vector<vector<string>> aux;
		aux = cluster;
		aux.emplace_back(records[i]);

		diff = information_loss(aux)
			- information_loss(cluster);
		if (diff < min || min == -1) {
			min = diff;
			best = i;
		}
	}

	return best;
}


