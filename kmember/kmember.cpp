#include "kmember.h"

int main(int argc, char** argv) {


	if (argc != 2) {
		cout << "\nInvalid arguments.\n"
			"Use ./kmember [data directory]\n\n"
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
	
	//vector<string> qidNames = {"Age", "Country", "Occupation"};
	vector<string> qidNames(qid_set.begin(), qid_set.end());

	// Read data file and hierarchy folders
	vector<string> headers;
	vector<int> catQids, numQids, allQids;
	vector<vector<string>> hierarchies, dataset,
			       transposedDataset;
	map<int, vector<vector<string>>> hierarchies_map;


	try {
		hierarchies_map = read_directory(fs::path(argv[1]),
					dataset, headers, K, qidNames,
					catQids);

		if (catQids.size() == 0) {
			cout << endl << "******************" << endl; 
			cout << "An error occured.\nCheck the qid "
				"names entered exists. They should be "
				"referenced\nin their respectives "
				"hierarchy files." << endl << endl;
			return -1;
		}
		// Compare headers and qids
		allQids = getQidsHeaders(headers, qidNames);
		set_difference(allQids.begin(), allQids.end(),
			catQids.begin(), catQids.end(),
        		inserter(numQids, numQids.begin())); 

	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}

	// Main algorithm
	vector<vector<string>> S = dataset;
	int r = randomRecord(S);


	map<int, vector<vector<string>>> res;
	vector<vector<string>> c;
	int count = 0;
	vector<string> aux = S[r];

	// 1st loop
	while ((int)S.size() >= K) {

		r = furthestRecord(aux, r, S,
			   hierarchies_map,
			   numQids, catQids);

		vector<vector<string>> c;
		c.emplace_back(S[r]);
		S.erase(S.begin() + r);

		while((int)c.size() < K) {
			r = find_best_record(S, c,
				hierarchies_map,
				numQids, catQids);
			c.emplace_back(S[r]);
			S.erase(S.begin() + r);
		}

		aux = c.back();
		res[count] = c;
		count += 1;
	}

	/*
	cout << "*******************" << endl;
	cout << "Count: ";
	cout << count << endl;
	for (int i=0; i < count; i++) {
		cout << "Count: ";
		cout << i << endl;
		for (const auto& entries : res[i]) {
			for (const auto& entry : entries) {
				cout << entry + ", ";
			}
			cout << endl;
		}
	}*/


	// 2nd loop
	int idx;
	while (S.size() > 0) {
		r = randomRecord(S);
		idx = find_best_cluster(res, S[r],
			hierarchies_map,
			numQids, catQids);
		res[idx].emplace_back(S[r]);
		S.erase(S.begin() + r);
	}


	// Print results
	cout << endl << endl; 
	cout << "Clusters or equivalence classes: ";
	cout << count << endl << endl;
	for (int i=0; i < count; i++) {
		cout << "Cluster " << i+1 << ':' << endl;
		for (const auto& entries : res[i]) {
			for (const auto& entry : entries) {
				cout << entry + ", ";
			}
			cout << endl;
		}
	}


	return 0;
}

