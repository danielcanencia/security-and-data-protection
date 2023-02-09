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

map<string, int> calculateQidFreqs(
	const vector<vector<string>> dataset, const int dim) {

	vector<vector<string>> records = dataset;

	// Calculate frequency list
	map<string, int> freqs;

	for (const auto& record : records) {
		try {
			freqs[record[dim]] += 1;
		} catch (...) {
			freqs[record[dim]] = 1;
		}
	}

	return freqs;
}

vector<int> calculateFreqs(
	const vector<vector<string>> dataset) {

	// Concatenate all attributes of each record
	vector<string> records = concatRecords(dataset);

	// Calculate frequency list
	map<string, int> freqMap;

	for (const auto& record : records) {
		try {
			freqMap[record] += 1;
		} catch (...) {
			freqMap[record] = 1;
		}
	}

	// Get all values from freqMap
	vector<int> freqs;
	for(map<string,int>::iterator it = freqMap.begin();
		it != freqMap.end(); it++) {
  		freqs.push_back(it->second);
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

vector<vector<vector<string>>> createClusters(vector<vector<string>> dataset,
					      vector<int> qids) {
	// Extract qid columns only
	vector<vector<string>> qids_dataset;
	for (size_t i=0; i < dataset.size(); i++) {
		vector<string> aux;
		for (const int& idx : qids)
			aux.emplace_back(dataset[i][idx]);

		qids_dataset.emplace_back(aux);
	}

	// Concatenate all attributes of each record
	vector<string> records = concatRecords(qids_dataset);

	// Get only unique records 
	vector<string> rcopy = records;
	sort(rcopy.begin(), rcopy.end());
	auto it = unique(rcopy.begin(), rcopy.end());
	rcopy.resize(distance(rcopy.begin(), it));

	// List from 0 to records.size()
	vector<int> indexes(records.size(), 0);
	iota(indexes.begin(), indexes.end(), 0);

	// Get elements positions in records vector
	map<string, vector<int>> cmap;
	for (size_t i=0; i < rcopy.size(); i++) {
		string val = rcopy[i];
		cmap[val] = {};

		vector<int> deletions;
		for (size_t j=0; j < indexes.size(); j++) {
			if (records[indexes[j]] == val) {
				cmap[val].emplace_back(indexes[j]);
				deletions.emplace_back(j);
			}
		}

		int n=0;
		for (const auto& idx : deletions) {
			indexes.erase(indexes.begin() + idx - n);
			n++;
		}
	}

	vector<vector<vector<string>>> clusters;
	for (const auto& [key, values] : cmap) {
		vector<vector<string>> cluster;
		for (const auto& idx : values)
			cluster.emplace_back(dataset[idx]);

		clusters.emplace_back(cluster);
	}

	return clusters;
}

