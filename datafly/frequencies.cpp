#include "frequencies.h"

vector<tuple<vector<int>, int>> calculateFreqsByidx(
	const vector<vector<string>> dataset) {

	// Concatenate all attributes of each record
	vector<string> records;
	string delimiter = "";
	for (size_t i=0; i < dataset.size(); i++) {
		records.emplace_back(accumulate(
			dataset[i].begin(), dataset[i].end(),
			delimiter));
	}

	for (const auto& a : records)
		cout << a << endl;

	// Calculate frequency list
	vector<tuple<vector<int>, int>> freqs;
	while (records.size() > 0) {
		string val = records.front();
		records.erase(records.begin());

		// Calculate record freq & records in cluster
		vector<int> deletions;
		int freq = 1;
		for (size_t j=0; j < records.size(); j++) {
			if (val == records[j]) {
				deletions.emplace_back(j);
				freq += 1;
			}
		}

		int n = 0;
		for (const int& idx : deletions) {
			records.erase(records.begin() + idx - n);
			n++;
		}

		freqs.emplace_back(make_tuple(deletions, freq));

	}

	return freqs;
}

vector<int> calculateFreqs(
	const vector<vector<string>> dataset) {

	// Concatenate all attributes of each record
	vector<string> records;
	string delimiter = "";
	for (size_t i=0; i < dataset.size(); i++) {
		records.emplace_back(accumulate(
			dataset[i].begin(), dataset[i].end(),
			delimiter));
	}

	/*for (const auto& a : records)
		cout << a << endl;*/

	// Calculate frequency list
	vector<int> freqs;
	while (records.size() > 0) {
		string val = records.front();
		records.erase(records.begin());

		// Calculate record freq & records in cluster
		vector<int> deletions;
		int freq = 1;
		for (size_t j=0; j < records.size(); j++) {
			if (val == records[j]) {
				deletions.emplace_back(j);
				freq += 1;
			}
		}

		int n = 0;
		for (const int& idx : deletions) {
			records.erase(records.begin() + idx - n);
			n++;
		}

		freqs.emplace_back(freq);

	}

	return freqs;
}


int findMostDistinctQid(const vector<vector<string>> dataset) {

	size_t cols = dataset[0].size();
	vector<vector<string>> values(cols);

	// Get firt value of every attribute
	for (size_t i=0; i < cols; i++)
		values[i].emplace_back(dataset[0][i]);

	// Every attribute has at least 1 distinct value
	vector<int> nvalues(cols, 1);

	// For all records
	for (size_t i=1; i < dataset.size(); i++) {

		// Check if the auxiliar list (values) contains
		// the attribute value
		for (int j=0; j < cols; j++) {
			if (find(values[j].begin(), values[j].end(),
				dataset[i][j]) == values[j].end()) {

				values[j].emplace_back(dataset[i][j]);
				nvalues[j]++;
			}
		}
	}

	/*for (const auto& q : nvalues)
		cout << to_string(q) + ", ";
	cout << endl;*/

	return distance(nvalues.begin(),
		max_element(nvalues.begin(),
			    nvalues.end()));
}

bool isKAnonSatisfied(const vector<vector<string>> dataset,
		      const int K) {
	cout << "Frequencies: ";
	for (const int& freq : calculateFreqs(dataset)) {
		cout << to_string(freq) + ", ";
		if (freq < K) {
			cout << endl;
			return false;
		}
	}
	cout << endl;

	return true;
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

