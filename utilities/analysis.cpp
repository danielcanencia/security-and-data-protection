#include "analysis.h"

/*
long double getNCP(const vector<vector<string>> cluster,
		   const map<int, vector<vector<string>> hierarchies,
		   const map<int, int> max, const map<int, int> min,
		   const vector<int> weights) {

	int ncp = 0;

	// Sum NCP for every qid attribute in cluster
	for (size_t i=0; i < cluster.size(); i++) {
		// Transposed entry showing all values of a qid
		if (max.count(i)) {
			// Numeric Attribute
			maxQid += max_element(cluster[i].begin(),
					cluster[i].end());
			minQid += min_element(cluster[i].begin(),
					cluster[i].end());
			ncp += ((maxQid - minQid) / (max[i] - min[i])) * weights[i];
			continue;
		}

		// Categorical Attribute
		long double card = getCard(cluster[i], hierarchy[i]);
		if (card != (long double)1)
			ncp += card / hierarchies[i].size();
	}

	return ncp;
}

vector<int> getNCPs(vector<vector<vector<string>>> clusters,
		    map<int, vector<vector<string>>> hierarchies,
		    vector<int> numQids, vector<int> weights,
		    vector<long double> cncps) {
	map<int, int> max;
	map<int, int> min;

	// Calculate maximum value of every qid in all clusters
	for (const auto& cluster : clusters) {
		for (const auto nqid : numQids) {
			// Numerical Attribute
			if (numQids. cluster[i]) {
				max[nqid] = max_element(cluster[i].begin(),
					     cluster[i].end());
				min[nqid] = min_element(cluster[i].begin(),
					     cluster[i].end());
				continue;
			}
		} 
	}

	vector<int> ncps;

	// Calculate NCP for every cluster
	for (size_t i=0; i < clusters.size(); i++) {
		ncps.emplace_back(getNCP(clusters[i], hierarchies,
				  max, min, weights));
	}

	return ncps;
}
*/

void printAnalysis(vector<vector<vector<string>>> clusters,
		   int numRecords,
                   map<int, vector<vector<string>>> hierarchies,
                   vector<int> qids, vector<double> weights,
                   vector<long double> cncps) {

	long double gcp = 0;

	for (size_t i=0; i < clusters.size(); i++) {
		gcp += (long double)(clusters[i].size() * cncps[i]);
	}
	gcp /= (qids.size() * numRecords);

	cout << "===> Analysis: " << endl;
	// Show only 3 decimals
	cout << "\t* GCP: ";
	cout << fixed << setprecision(3) << gcp << endl;
}

