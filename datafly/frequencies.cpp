#include "frequencies.h"


vector<tuple<string, int>> concatRecordsByIdx(
	const vector<vector<string>> dataset) {

        vector<tuple<string, int>> records;

        // Concatenate all elements
        for (size_t i=0; i < dataset.size(); i++) {
                records.emplace_back(make_tuple(accumulate(
                        dataset[i].begin(), dataset[i].end(),
                        string()), i));
        }

        return records;
}

vector<string> concatRecords(const vector<vector<string>> dataset) {
	vector<string> records;

	// Concatenate all elements 
        for (size_t i=0; i < dataset.size(); i++) {
                records.emplace_back(accumulate(
                        dataset[i].begin(), dataset[i].end(),
                        string()));
        }

	return records;
}

vector<int> calculateFreqs(
	const vector<vector<string>> dataset) {

	// Concatenate all attributes of each record
	vector<string> records = concatRecords(dataset);

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
		for (size_t j=0; j < cols; j++) {
			if (find(values[j].begin(), values[j].end(),
				dataset[i][j]) == values[j].end()) {

				values[j].emplace_back(dataset[i][j]);
				nvalues[j]++;
			}
		}
	}

	return distance(nvalues.begin(),
		max_element(nvalues.begin(),
			    nvalues.end()));
}

