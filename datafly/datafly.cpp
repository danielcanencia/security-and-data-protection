#include "datafly.h"

int main(int argc, char** argv) {


	if (argc != 2) {
		cout << "\nInvalid arguments.\n"
			"Use ./datafly.out [data directory]\n\n"
			"* Make sure your data directory meets the following structure:\n"
			"  (check dataset folder for an example)\n\n"
			" |-- [data directory]\n"
			"      |\n"
			"      |-- csv input file\n"
			"      |-- hierarchies\n"
			"           |\n"
			"           |-- hierarchy tables as csv files\n"
			"           |-- ....\n"
		<< endl;
		return -1;
	}

	// Read input
	string line;
	cout << "Insert K: ";
	getline(cin, line);
	const int K = stoi(line);

	/*set<string> qid_set;
	cout << "Number of qids: ";
	getline(cin, line);
	const int nqids = stoi(line);
	for (int i=0; i < nqids; i++) {
		cout << "Enter qid " << i << ": ";
		string qid;
		getline(cin, qid);
		qid_set.insert(qid);
	}*/
	
	vector<string> qidNames = {"Age", "Country", "Occupation"};
	//vector<string> qidNames(qid_set.begin(), qid_set.end());

	// Read data file and hierarchy folders
	vector<string> headers;
	vector<int> qids;
	vector<vector<string>> qids_dataset, dataset;
	map<int, vector<vector<string>>> hierarchies_map;


	try {
		hierarchies_map = read_directory(fs::path(argv[1]),
					dataset, headers, K, qidNames,
					qids);

		if (qids.size() == 0) {
			cout << endl << "******************" << endl; 
			cout << "An error occured.\nCheck the qid "
				"names entered exists. They should be "
				"referenced\nin their respectives "
				"hierarchy files." << endl << endl;
			return -1;
		}

	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}


	// Main algorithm

	// Obtain a subset of dataset containing only qids
	for (size_t i=0; i < dataset.size(); i++) {
		vector<string> aux;
		for (const int& idx : qids) {
			aux.emplace_back(dataset[i][idx]);
		}
		qids_dataset.emplace_back(aux);
	}

	// 1. Create a hierarchy tree for every qid
	vector<Tree> trees;
	for (const int& val : qids) {
		trees.emplace_back(Tree(hierarchies_map[val]));
		//tree.print();
	}

	// 2. Calculate frecuency for every qid
	/*map<int, FreqList> freq;
	for (const int& idx : allQids) {
		// Check if qid is categorical or numeric
		// 1 => numeric, 0 => categorical
		//qidType = find(numQids.begin(), numQids.end(), idx)
		//		!= numQids.end());
		// Add a freqList object to freq vector
		//freq.emplace_back(freqList(tranposeDataset[idx], qidType);
	}*/

	for (const auto& a : qids_dataset) {
		for (const auto& b : a)
			cout << b + ", ";
		cout << endl;
	}

	// 2. Calculate record frecuency 
	/*vector<int> freqs = calculateFreqs(qids_dataset); //, allQids);
	for (size_t i=0; i < freqs.size(); i++) {
		cout << to_string(freqs[i]) + ", ";
	}
	cout << endl;*/

	int qid, idx;
	vector<vector<string>> table;
	// 2&3. Calculate frequencies & Check if KAnonimity is satisfied
	while (!isKAnonSatisfied(qids_dataset, K)) {

		// 3.1 Find qid with the most distinct values
		idx = findMostDistinctQid(qids_dataset);
		cout << idx << endl;

		// 3.2 Generalize all qid values in freq
		generalizeQid(qids_dataset, idx, trees[idx]);

	}


	// 4. Supress records which are not K-Anonymous (< K times)
	/*supressRecords(dataset);

	// Update original dataset with generalized values 
	for (size_t i=0; i < dataset.size(); i++) {
		for (size_t j=0; j < qids_dataset.size(); j++) {
			dataset[i][idx] = qids_dataset[allQids[j]];
		}
	}*/

	/*
	for (const auto& entries : dataset) {
		for (const auto& entry : entries) {
			cout << entry + ", ";
		}
		cout << endl;
	*/

	// 5. Write anonymized table
	//writeAnonymizedTable(headers, dataset, allQids);

	return 0;
}

