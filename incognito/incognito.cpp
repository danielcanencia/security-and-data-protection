#include "incognito.h"

int main(int argc, char** argv) {

	if (argc != 2) {
		cout << "\nInvalid arguments.\n"
			"Use ./incognito [data directory] [k]\n\n"
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
	const int nqids = readNumberOfQids();
	vector<string> qidNames;
	vector<string> confAttNames;
	try {
		// Read Qid Names
		qidNames = readQidNames(nqids);
		// Conf Atts Names
		confAttNames = readConfidentialAttNames();
	} catch (const char* e) {
		cout << e << endl;
		return -1;
	}

	// Read csv data file
	vector<string> headers;
	vector<int> qids, confAtts;
	vector<vector<string>> dataset, transposedDataset;
	map<int, vector<vector<string>>> hierarchiesMap;
	map<int, vector<vector<string>>> transposedHierarchyMap;

	try {
		hierarchiesMap = readDirectory(fs::path(argv[1]),
					dataset, headers, qidNames, confAttNames,
					qids, confAtts, true);
		sort(qids.begin(), qids.end());
		if (qids.size() < qidNames.size()) {
			cout << endl << "******************" << endl; 
			cout << "An error occured.\nCheck the qid "
				"names entered exists. They should be "
				"referenced\nin their respectives "
				"hierarchy files." << endl << endl;
			return -1;
		}

		if (confAtts.size() < confAttNames.size()) {
			cout << endl << "******************" << endl; 
			cout << "An error occured.\nCheck the confidential "
				"attributte names entered exists.\nThey should be "
				"referenced in their respectives "
				"hierarchy files." << endl << endl;
			return -1;
		}

		for (const auto& qid : qids) {
			transposedHierarchyMap[qid] = transpose(hierarchiesMap[qid]);
		}

		transposedDataset = transpose(dataset);
	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}


	// Read Parameters
	int K, L;
	long double P;
	readParameters(dataset.size(), confAttNames.size(), K, L, P);
	// Read Weights
	vector<double> weights = readWeights(nqids, qidNames);
	// Ask for desired qid types to be used on metrics
	vector<int> numMetricsQids, catMetricsQids;
	tuple<vector<int>, vector<int>> metricsQids =
		readMetricsQids({}, qids, headers);
	numMetricsQids = get<0>(metricsQids);
	catMetricsQids = get<1>(metricsQids);


	// Measure Execution Time
	auto start = chrono::high_resolution_clock::now();
	// *********************************
	// Main algorithm
	auto resTuple = incognito(dataset, hierarchiesMap, qids,
							  confAtts, K, L, P);
	vector<vector<string>> result = get<0>(resTuple);
	vector<vector<vector<string>>> clusters = get<1>(resTuple);
	// *********************************
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
	cout << endl << "===> Incognito Execution Time: ";
	cout << duration.count() << " seconds" << endl;

	cout << "===> Number of clusters: ";
	cout << clusters.size() << endl;

	// Write anonymized table
	writeAnonymizedTable(fs::path(argv[1]), headers, result, K, L, P);


	// Create a hierarchy tree for every qid
	vector<Tree> trees;
	for (const int& val : qids) {
		trees.emplace_back(Tree(transposedHierarchyMap[val]));
	}

	// METRICS
	cout << "===> Analysis: " << endl;
	// Convert hierarchy tree map into a list
	map<int, Tree> treeMap;
	for (size_t i=0; i < qids.size(); i++) {
		treeMap[qids[i]] = trees[i];
	}

	// GCP
	try {
		// 1. Precalculate NCP for every qid value included in every cluster
		vector<long double> cncps = calculateNCPS(
			clusters, weights, qids, numMetricsQids, treeMap);

		// 2. Calculate GCP
		calculateGCP(clusters, dataset.size(), qids, cncps);
	} catch (const char* e) {
		cout << e << endl;
		return -1;
	}

	// DM
	calculateDM(clusters, dataset.size(), K, L, P);

	// CAvg
	calculateCAVG(clusters, dataset.size(), K, L, P);

	// GenILoss
	try {
		calculateGenILoss(transpose(result), treeMap, qids,
						  catMetricsQids, numMetricsQids,
						  dataset.size());
	} catch (const char* e) {
		cout << e << endl;
		return -1;
	}

	return 0;
}

