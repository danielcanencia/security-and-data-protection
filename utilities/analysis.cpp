#include "analysis.h"

void printAnalysis(vector<vector<vector<string>>> clusters,
		   int numRecords, vector<int> qids,
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

