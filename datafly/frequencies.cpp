#include "frequencies.h"

vector<int> calculateFreqsNR(const vector<vector<string>> dataset) {

	// Concatenate all attributes of each record
	vector<string> records;
	string delimiter = ", ";
	for (size_t i=0; i < dataset.size(); i++) {
		records.emplace_back(accumulate(
			dataset[i].begin(), dataset[i].end(),
			delimiter));
	}

	// Calculate frequency list
	vector<int> freqs;
	string val;
	while(records.size() > 0) {
		val = records.front();
		records.erase(records.begin());

		// Calculate record freq
		int freq = 1;
		for (size_t j=0; j < records.size(); j++) {
			if (val == records[j]) {
				records.erase(records.begin() + j);
				freq += 1;
			}
		}
		freqs.emplace_back(freq);
	} 

	return freqs;
}

map<int, int> calculateFreqs(const vector<vector<string>> dataset) {

	// Concatenate all attributes of each record
	vector<string> records;
	string delimiter = ", ";
	for (size_t i=0; i < dataset.size(); i++) {
		records.emplace_back(accumulate(
			dataset[i].begin(), dataset[i].end(),
			delimiter));
	}

	// Calculate frequency list
	map<int, int> freqs;
	vector<int> cluster;
	string val;
	for (size_t i=0; i < records.size(); i++) {
		vector<int> cluster;

		val = records[i];
		cluster.emplace_back(i);

		// Calculate record freq & records in cluster
		int freq = 1;
		for (size_t j=0; j < records.size(); j++) {
			if (val == records[j]) {
				cluster.emplace_back(j);
				freq += 1;
			}
		}

		// Assign the same frequency to all elements
		// in cluster
		for (const auto& idx : cluster) {
			freqs[idx] = freq;
		}
	}

	return freqs;
}


int findMostDistinctQid(const vector<vector<string>> dataset) {

	int cols = dataset[0].size();
	vector<int> nvalues(cols, 1);
	vector<vector<string>> values;
	for (int i=0; i < cols; i++) {
		values[i].emplace_back(dataset[0][i]);
	}

	// For all records
	for (size_t i=1; i < dataset.size(); i++) {

		// Check if the auxiliar list (values) contains
		// the attribute value
		for (int j=0; j < cols; j++) {
			if (find(values[j].begin(), values[j].end(),
				dataset[i][j]) != values[j].end()) {

				values[j].emplace_back(dataset[i][j]);
				nvalues[j]++;
			}
		}
	}

	for (const auto& q : nvalues)
		cout << to_string(q) + ", ";
	cout << endl;

	return distance(nvalues.begin(),
		max_element(nvalues.begin(),
			    nvalues.end()));
}


/*
  Full-Domain Generalization
*/
void generalizeQid(vector<vector<string>> dataset,
		   int qid, Tree tree) {

	/*vector<string> aux;
	for (size_t i=0; i < dataset.size(); i++) {
		aux.emplace_back(dataset[i][qid]);
	}

	vector<string> gen = tree.getNextGens(aux);*/

	for (size_t i=0; i < dataset.size(); i++) {
		//dataset[i][qid] = gen;
		dataset[i][qid] = tree.getNextGen(dataset[i][qid]);
	}

	//transform(dataset[qid].cbegin(), dataset[qid].cend(),
        //          dataset[qid].begin(),
        //          [](unsigned string s) { return gen; });

}

void supressRecords(vector<vector<string>> dataset, int K,
		    vector<int> frequencies) {

	for (size_t i=0; i < dataset.size(); i++) {
		if (frequencies[i] < K)
			dataset.erase(dataset.begin() + i);
	}
}

