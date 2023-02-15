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

	/*vector<int> numMetricsQids;
	string question = "Do you want to treat some hierarchical attributes as "
			 		  "numerical (will only be used on metrics) [Y(y)/N(n)]: ";
	cout << question;
	cin >> answer;
	keep = false;
	while(!keep) {
		switch(answer) {
			case 'Y':
			case 'y':
				cout << "Enter number printed between brackets: ";
				for (size_t i=0; i < qidNames.size(); i++)
					cout << qidNames[i] + "(" + to_string(i) + ") ";
				cout << endl;
				char aux;
				while (1) {
					cout << " [enter q to quit] >> ";
					cin >> aux;
					if (aux == 'q')
						break;
					numMetricsQids.emplace_back((int)aux - 48);
				}
				keep = true;
				break;
			case 'N':
			case 'n':
				keep = true;
				break;
			default:
				cout << question;
				cin >> answer;
		}
	}*/

	// Read qid names
	const int nqids = readNumberOfQids();
	vector<string> qidNames = readQidNames(nqids);

	// Read data file and hierarchy folders
	vector<string> headers;
	vector<int> catQids, numQids, allQids;
	vector<int> isQidCat;
	map<int, vector<vector<string>>> hierarchies_map;
	vector<vector<string>> dataset;

	try {
		hierarchies_map = read_directory(fs::path(argv[1]),
					dataset, headers, qidNames,
					catQids, false);
		sort(catQids.begin(), catQids.end());

		// Compare headers and qids
		allQids = getQidsHeaders(headers, qidNames);
		if (catQids.size() == 0 || allQids.size() < qidNames.size()) {
			cout << "An error occured.\nCheck the qid "
				"names entered exists. They should be "
				"referenced\nin their respectives "
				"hierarchy files." << endl << endl;
			return -1;
		}

		sort(allQids.begin(), allQids.end());
		set_difference(allQids.begin(), allQids.end(),
			catQids.begin(), catQids.end(),
        		inserter(numQids, numQids.begin())); 

		// Build a vector containing qid types
		for (const auto& qid : allQids) {
			if (find(catQids.begin(), catQids.end(), qid)
				!= catQids.end()) {
				isQidCat.emplace_back(1);
				continue;
			}
			isQidCat.emplace_back(0);
		}
	} catch (const char* e) {
		cout << e << endl; 
		return -1;
	}

	// Read input
	int K, L, P;
	readParameters(dataset.size(), K, L, P);
	vector<double> weights = readWeights(nqids, qidNames);
	//vector<string> qidNames = {"Age", "Country", "Occupation" };
	//vector<string> qidNames = {"Salary", "Occupation", "Country"};
	/*vector<string> qidNames = {
		"Education", "Marital-status",
		"Native-country", "Occupation", "Race", "Relationship",
		"Salary", "Sex", "Workclass"
	};*/
	/*vector<string> qidNames = {
		"Workclass", "Education"
	};*/

	// Ask for desired qid types to be used on metrics
	vector<int> numMetricsQids, catMetricsQids;
	tuple<vector<int>, vector<int>> metricsQids =
		readMetricsQids(numQids, catQids, qidNames);
	numMetricsQids = get<0>(metricsQids);
	catMetricsQids = get<1>(metricsQids);


	// *********************************
	// Main algorithm
	vector<vector<string>> result = mondrian(dataset, hierarchies_map,
		allQids, isQidCat, K);
	// *********************************

	// METRICS
	cout << "===> Analysis: " << endl;
	// Create a hierarchy tree for every qid
	map<int, Tree> trees;
	for (const int& i : catQids) {
		trees[i] = Tree(hierarchies_map[i]);
	}

	// Create equivalence classes or clusters
	vector<vector<vector<string>>> clusters = createClusters(result, allQids);

	// GCP
	// 	1. Precalculate NCP for every qid value included in every cluster
	// Count total number of distinct qid values in dataset
	vector<long double> cncps = calculateNCPS(clusters, weights,
					allQids, numMetricsQids, trees);
	// 	2. Calculate GCP
	printAnalysis(clusters, dataset.size(), allQids, cncps);

	// DM
	calculateDM(clusters, dataset.size(), K);

	// CAvg
	calculateCAVG(clusters, dataset.size(), K);

	// GenILoss
	calculateGenILoss(transpose(result), trees, catMetricsQids,
					  numMetricsQids, dataset.size(), K);

	return 0;
}

