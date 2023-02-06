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
	// K
	string line;
	cout << "Insert K: ";
	getline(cin, line);
	const int K = stoi(line);

	// Qids
	set<string> qid_set;
	cout << "Number of qids: ";
	getline(cin, line);
	const int nqids = stoi(line);
	for (int i=0; i < nqids; i++) {
		cout << "Enter qid " << i << ": ";
		string qid;
		getline(cin, qid);
		qid_set.insert(qid);
	}
	if ((int)qid_set.size() != nqids) {
		cout << "Input Error: Qids should be unique.";
		cout << "Check if you repeated some of them" << endl;
		return 1;
	}
	vector<string> qidNames(qid_set.begin(), qid_set.end());
	//vector<string> qidNames = {"Age", "Occupation"};
	//vector<string> qidNames = {"Age", "Country", "Occupation"};
	/*vector<string> qidNames = {
		"Education", "Marital-status",
		"Native-country", "Occupation", "Race", "Relationship",
		"Salary", "Sex", "Workclass"
	};*/


	// Weights
	vector<double> weights = {};
	cout << "Do you want to use weights (will only be used on analysis) [Y(y)/N(n)]: ";
	char answer;
	cin >> answer;
	bool keep = false;
	while(!keep) {
		switch(answer) {
			case 'Y':
			case 'y':
				for (int i=0; i < nqids; i++) {
					cout << "Weight for qid " << i << ": ";
					float weight;
					cin >> weight;
					weights.emplace_back(weight);
				}
				if (accumulate(weights.begin(), weights.end(),
					(float)0) != (float)1) {
					cout << "Input Error: Weights must sum 1" << endl;
					weights.clear();
					continue;
				}
				keep = true;
				break;
			case 'N':
			case 'n':
				keep = true;
				break;
			default:
				cout << "Do you want to use weights ";
				cout << "(will only be used on analysis) ";
				cout << "[Y(y)/N(n)]: ";
				cin >> answer;
		}
	}


	// Read data file and hierarchy folders
	vector<string> headers;
	vector<int> qids;
	vector<vector<string>> qids_dataset, dataset;
	map<int, vector<vector<string>>> hierarchies_map;

	try {
		hierarchies_map = read_directory(fs::path(argv[1]),
					dataset, headers, K, qidNames,
					qids);
	} catch (const char* e) {
		cout << e << endl;
		return -1;
	}

	if (qids.size() < qidNames.size()) {
		cout << "ERROR:\n\tCheck the qid "
			"names entered exists. They should be "
			"referenced\n\tin their respectives "
			"hierarchy files." << endl << endl;
		return -1;
	}
	sort(qids.begin(), qids.end());


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
	}

	int idx;
	// 2&3. Calculate frequencies & Check if KAnonimity is satisfied
	while (!isKAnonSatisfied(qids_dataset, K)) {

		// 3.1 Find qid with the most distinct values
		idx = findMostDistinctQid(qids_dataset);

		// 3.2 Generalize all qid values in freq
		try {
			generalizeQid(qids_dataset, idx, trees[idx]);
		} catch (const char* e) {
			cout << e << endl;
			return 1;
		}
	}

	// 4. Supress records which are not K-Anonymous (< K times)
	supressRecords(qids_dataset, K);

	// Update original dataset with generalized values
	for (size_t i=0; i < dataset.size(); i++) {
		for (size_t j=0; j < qids.size(); j++) {
			dataset[i][qids[j]] = qids_dataset[i][j];
		}
	}

	// 5. Write anonymized table
	// Changed headers for non alterated ones
	writeAnonymizedTable(fs::path(argv[1]), headers, dataset, K);

	// End of main algorithm

	// GCP Analysis
	// 1. Create equivalence classes or clusters
	vector<vector<vector<string>>> clusters = createClusters(dataset, qids);
   
	// 2. Especify weights, if any (Already entered by user)
	//vector<double> weights(qids.size(), 1.0/qids.size());

	// 3. Calculate NCP for every qid value included in every cluster
	// Convert list into map
	map<int, Tree> treeMap;
	for (size_t i=0; i < qids.size(); i++) {
		treeMap[qids[i]] = trees[i];
	}
	// Calculate NCP
	vector<long double> cncps = calculateNCPS(
		clusters, weights, qids, {}, treeMap);

	// 4. Calculate GCP
	printAnalysis(clusters, dataset.size(), qids, cncps);

	return 0;
}

