#include "mondrian.h"

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
	/*set<string> qid_set;
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
	}*/
	//vector<string> qidNames(qid_set.begin(), qid_set.end());
	vector<string> qidNames = {"Age", "Country", "Occupation"};

	// Weights
	/*vector<double> weights = {};
	cout << "Do you want to use weights (will only be used on analysis) [Y(y)/N(n)]: ";
	char answer;
	cin >> answer;
	bool keep;
	while(!keep) {
		switch(answer) {
			case 'Y':
			case 'y':
				for (int i=0; i < nqids; i++) {
					cout << "Weight for qid " << i << ": ";
					double weight;
					cin >> weight;
					weights.emplace_back(weight);
				}
				if (accumulate(weights.begin(), weights.end(),
					(float)0) != (float)1) {
					cout << "Input Error: Weights must sum 1" << endl;
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
	}*/


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
		cout << endl << "******************" << endl; 
		cout << "An error occured.\nCheck the qid "
			"names entered exists. They should be "
			"referenced\nin their respectives "
			"hierarchy files." << endl << endl;
		return -1;
	}
	sort(qids.begin(), qids.end());


	// *********************************
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

	/*vector<int> allowedCuts(qids.size(), 1);
	int dim = choose_dim(qids_dataset);	
	find_median(qids_dataset, dim);*/

	// 2. Anonymize whole initial partition
	vector<string> gens;
	vector<int> numLeaves;
	for (size_t i=0; i < qids.size(); i++) {
		gens.emplace_back(trees[i].root);
		numLeaves.emplace_back(
			trees[i].getNumSubTreeLeaves(trees[i].root));
	}

	Partition partition(dataset, gens, numLeaves, qids, trees, K);
	vector<Partition> resDataset = evaluate(partition);

	// End of main algorithm
	// *********************************


	// GCP Analysis
	// 1. Create equivalence classes or clusters
	//vector<vector<vector<string>>> clusters = createClusters(qids_dataset, qids);

	// 2. Especify weights, if any (Already entered by user)
	//vector<double> weights = {0.3, 0.4, 0.3};

	// 3. Precalculate NCP for every qid value included in every cluster
	// Count total number of distinct qid values in dataset
	/*vector<long double> cncps;
	int nweights = weights.size();
	for (const auto& cluster : clusters) {
		vector<vector<string>> tcluster = transpose(cluster);

		// Qids Att Values
		long double ncp = 0.0;

		for (size_t i=0; i < qids.size(); i++) {
			// Calculate NCP fot qid values 
			long double card = trees[i].getNCP(tcluster[i]);
			if (card == 1)
				continue;

			card /= trees[i].getNumLeaves();
			double weight = nweights > 0 ? weights[i] : 1;
			ncp += (long double)weight * card;
		}
		ncp *= nweights ? qids.size() : 1;
		cncps.emplace_back(ncp);
	}
	*/

	// 3. Calculate GCP
	//printAnalysis(clusters, dataset.size(), qids, cncps);

	return 0;
}

