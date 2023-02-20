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
	// Read qid names
	const int nqids = readNumberOfQids();
	vector<string> qidNames = readQidNames(nqids);


	//vector<string> qidNames(qid_set.begin(), qid_set.end());
	//vector<string> qidNames = {"Age", "Occupation"};
	//vector<string> qidNames = {"Age", "Country", "Occupation"};
	/*vector<string> qidNames = {
		"Education", "Marital-status",
		"Native-country", "Occupation", "Race", "Relationship",
		"Salary", "Sex", "Workclass"
	};*/

	// Read data file and hierarchy folders
	vector<string> headers;
	vector<int> qids, confAtts;
	vector<vector<string>> qidsDataset, dataset;
	map<int, vector<vector<string>>> hierarchiesMap;

	try {
		hierarchiesMap = read_directory(fs::path(argv[1]),
					dataset, headers, qidNames, {},
					qids, confAtts, false);

		if (qids.size() < qidNames.size()) {
			cout << "ERROR:\n\tCheck the qid "
				"names entered exists. They should be "
				"referenced\n\tin their respectives "
				"hierarchy files." << endl << endl;
			return -1;
		}
		sort(qids.begin(), qids.end());
	} catch (const char* e) {
		cout << e << endl;
		return -1;
	}

	// Read Parameters
	const int K = readParameter("k-anonymity", "K", dataset.size());
	// Read Weights
	vector<double> weights = readWeights(nqids, qidNames);
	// Ask for desired qid types to be used on metrics
	vector<int> numMetricsQids, catMetricsQids;
	tuple<vector<int>, vector<int>> metricsQids =
		readMetricsQids({}, qids, qidNames);
	numMetricsQids = get<0>(metricsQids);
	catMetricsQids = get<1>(metricsQids);

	// *********************************
	// Main algorithm
	auto resTuple = datafly(dataset, hierarchiesMap, qids, K);
	vector<vector<string>> result = get<0>(resTuple);
	vector<vector<vector<string>>> clusters = get<1>(resTuple);
	if (result.size() == 0 || clusters.size() == 0)
		return 1;
	// *********************************

	// Write anonymized table
	// Changed headers for non alterated ones
	writeAnonymizedTable(fs::path(argv[1]), headers, result, K, -1, -1);


	// METRICS
	cout << "===> Analysis: " << endl;
	// Create a hierarchy tree for every qid
	map<int, Tree> trees;
	for (size_t i=0; i < qids.size(); i++) {
		trees[i] = Tree(hierarchiesMap[i]);
	}

	// GCP
	// 1. Precalculate NCP for every qid value included in every cluster
	vector<long double> cncps = calculateNCPS(clusters, weights,
		qids, numMetricsQids, trees);

	// 2. Calculate GCP
	calculateGCP(clusters, dataset.size(), qids, cncps);

	// DM
	calculateDM(clusters, dataset.size(), K, -1, -1);

	// CAvg
	calculateCAVG(clusters, dataset.size(), K, -1, -1);

	// GenILoss
	calculateGenILoss(transpose(result), trees, qids,
					  catMetricsQids, numMetricsQids, dataset.size());

	return 0;
}

