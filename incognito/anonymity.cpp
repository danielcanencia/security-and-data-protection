#include "anonymity.h"


bool isSplitKAnonymous(vector<vector<string>> split,
					   const int K) {
	return (int)split.size() >= K;
}

bool isSplitLDiverse(vector<vector<string>> split,
                     vector<int> confAtts, const int L) {
	vector<map<string, int>> freqs(confAtts.size());
	string key;

	for (const auto& entry : split) {
		for (size_t j = 0; j < confAtts.size(); j++) {
			key = entry[confAtts[j]];
			try {
				freqs[j][key] += 1;
			} catch (...) {
				freqs[j][key] = 1;
			}
		}
	}

	// Every confidential attribute should have, at least,
	// l well represented values 
	for (const map<string, int>& attFreq : freqs) {
		// Get all values from  map
		for (const auto& [k, v] : attFreq) {
			if (v < L)
				return false;
		}
	}

	return true;
}

bool isSplitTClose(vector<vector<string>> split, vector<vector<string>> data,
                   vector<int> confAtts, const long double P) {

	if (split.size() == 0)
		return false;

	// Generate a P frequency map for
	// every confidential attribute
	string key;
	vector<map<string, int>> splitMaps(confAtts.size());
	for (const auto& entry : split) {
		for (size_t j = 0; j < confAtts.size(); j++) {
			key = entry[confAtts[j]];
			try {
				splitMaps[j][key] += 1;
			} catch (...) {
				splitMaps[j][key] = 1;
			}
		}
	}

	vector<map<string, int>> dataMaps(confAtts.size());
	vector<set<string>> valueSets(confAtts.size());
	for (const auto& entry : data) {
		for (size_t j = 0; j < confAtts.size(); j++) {
			key = entry[confAtts[j]];
			try {
				dataMaps[j][key] += 1;
			} catch (...) {
				dataMaps[j][key] = 1;
			}
			valueSets[j].insert(key);
		}
	}

	// P and Q size
	int pSize, qSize;
	pSize = split.size();
	qSize = data.size();

	// Calculate EMD for every confidential attribute
	for (size_t i=0; i < confAtts.size(); i++) {
		long double emd = 0;
		for (const auto& entry : valueSets[i]) {
			if (splitMaps[i][entry]) {
				// Entry present in P
				// EMD(Pi, Qi) = abs(Pi/Pi probability in P -
				//					 Qi/Qi probability in Q)
				emd += abs((long double) splitMaps[i][entry]/pSize -
					   (long double) dataMaps[i][entry]/qSize);
			} else {
				// Not present in P
				// EMD(Pi, Qi) = abs(0 - Qi/Qi probability in Q)
				emd += (long double) dataMaps[i][entry]/qSize;
			}
		}
		// EMD(P, Q) = sum(sum(pj - qj)) / (m - 1)
		emd /= (qSize - 1);
		cout << emd << endl;
		// Check if partition is tclose
		if (emd > P)
			return false;
	}

	return true;
}

bool isSplitValid(vector<vector<vector<string>>> splits,
				  vector<vector<string>> dataset, vector<int> confAtts,
				  const int K, const int L, const long double P) {
	bool kanonymity, ldiversity, tcloseness;
	kanonymity = ldiversity = tcloseness = true;

	if (splits.size() == 0)
		return false;

	if (K > 0) {
		for (const auto& split : splits) {
			if (!isSplitKAnonymous(split, K))
				kanonymity = false;
		}
	}
	if (L > 0) {
		for (const auto& split : splits) {
			if(!isSplitLDiverse(split, confAtts, L))
				ldiversity = false;
		}
	}
	if (P > 0) {
		for (const auto& split : splits) {
			if (!isSplitTClose(split, dataset, confAtts, P))
				tcloseness = false;
		}
	}

	return kanonymity && ldiversity && tcloseness;
}
