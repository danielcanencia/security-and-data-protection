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
	}

	// If records contains just
	// the record 'record' return given index
	if (furthestIdx == -1)
		return index;
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
	// Numeric Values
	for (const auto& idx : info.getNumQids()) {
		v1 = r1[idx];
		v2 = r2[idx];

		numSum += numDistance(stold(v1),
				stold(v2),
				info.getMaxDomSize(idx));
	}

	// Categorical Values
	for (const auto& idx : info.getCatQids()) {
		v1 = r1[idx];
		v2 = r2[idx];

		// Categorical value
		catSum += catDistance(v1, v2, info, idx);
	}

	return numSum + catSum;
}

long double informationLoss(vector<vector<string>> records,
			     map<int, vector<vector<string>>> hierarchies,
		   	     const vector<int> numQids,
		   	     const vector<int> catQids) {
	int e = records.size();
	vector<long double> numValues, catValues;

	cout << "E: "; cout << e << endl;
	// Numeric Attributes
	for (const int& idx : numQids) {
		cout << "Idx: "; cout << idx << endl;
		long double max, min, aux;
		max = min = -1;
		vector<long double> numericDomain;
		for (const vector<string>& record : records) {
			cout << record[idx] << endl;
			aux = stold(record[idx]);
			numericDomain.emplace_back(aux);	

			if (aux > max)
				max = aux;
			if (aux < min || min == -1)
				min = aux;
		}

		unique(numericDomain.begin(),
		       numericDomain.end());

		numValues.emplace_back((max - min) / numericDomain.size()); 
	}
  
	// Categorical Attributes
	Info info(records, hierarchies, vector<int>{}, catQids);

	for (const int& idx : catQids) {
		catValues.emplace_back(
			info.lowestCommonAncestor(idx) /
			info.getTreeHeight(idx));
	}

	// Sum all values in every vector
	auto r1 = reduce(numValues.begin(), numValues.end());
	auto r2 = reduce(catValues.begin(), catValues.end());

	return e * (r1 + r2);
}

int findBestRecord(vector<vector<string>> records,
		     vector<vector<string>> cluster,
		     map<int, vector<vector<string>>> hierarchies,
		     const vector<int> numQids, const vector<int> catQids,
			 const int confAtt, const int L,
			 const vector<string> sensitiveValues,
			 const int diversityPenalty, const int diversity) {
	long double min = -1, diff;
	int best;

	for (size_t i=0; i < records.size(); i++) {
		vector<vector<string>> aux;
		aux = cluster;
		aux.emplace_back(records[i]);

		long double auxDiff = informationLoss(aux, hierarchies,
											   numQids, catQids)
							 - informationLoss(cluster, hierarchies,
												numQids, catQids);
		// K Anonymity
		diff = auxDiff;
		if (L != -1) {
			string majorClass = majorityClass(cluster, confAtt);
			string recordClass = records[i][confAtt];
			// Equal Diversity Metric
			if (diversity == 0) {
				if (isDiverse(cluster, confAtt, L))
					diff = auxDiff;
				else if (majorClass != recordClass)
					diff = auxDiff + diversityPenalty;
			}
			// Sensitive Diversity Metric
			else if (diversity) {
				if (isDiverse(cluster, confAtt, L))
					diff = auxDiff;
				else if (isSensitive(majorClass, confAtt, cluster)
						 && majorClass != recordClass)
					diff = auxDiff + diversityPenalty;
			}
		}
		if (diff < min || min == -1) {
			min = diff;
			best = i;
		}
	}

	return best;
}

int findBestCluster(map<int, vector<vector<string>>> clusters,
		      vector<string> record,
		      map<int, vector<vector<string>>> hierarchies,
		      const vector<int> numQids,
		      const vector<int> catQids) {
	long double min = -1, diff;
	int best;

	for (size_t i=0; i < clusters.size(); i++) {
		vector<vector<string>> aux;
		aux = clusters[i];
		aux.emplace_back(record);

		diff = informationLoss(aux, hierarchies,
					numQids, catQids)
			- informationLoss(clusters[i], hierarchies,
				    	numQids, catQids);
		if (diff < min || min == -1) {
			min = diff;
			best = i;
		}
	}

	return best;
}
